#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>
#include <iostream>

using namespace std;

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapFileHeader {
    char bitmapSignatureBytes[2] = {'B', 'M'};
    unsigned int sizeOfBitmapFile;
    uint32_t reservedBytes = 0;
    unsigned int pixelDataOffset;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    unsigned int sizeOfThisHeader;
    int width;
    int height;
    uint16_t numberOfColorPlanes;
    uint16_t colorDepth;
    uint32_t compressionMethod;
    uint32_t rawBitmapDataSize;
    int horizontalResolution;
    int verticalResolution;
    int colorTableEntries;
    int importantColors;
}
PACKED_STRUCT_END

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    BitmapFileHeader file_header_;
    file_header_.sizeOfBitmapFile = image.GetHeight() * GetBMPStride(image.GetWidth()) + 54;
    file_header_.reservedBytes = 0;
    file_header_.pixelDataOffset = 54;

    BitmapInfoHeader info_header_;
    info_header_.sizeOfThisHeader = 40;
    info_header_.width = image.GetWidth();
    info_header_.height = image.GetHeight();
    info_header_.numberOfColorPlanes = 1;
    info_header_.colorDepth = 24;
    info_header_.compressionMethod = 0;
    info_header_.rawBitmapDataSize = image.GetHeight() * GetBMPStride(image.GetWidth());
    info_header_.horizontalResolution = 11811;
    info_header_.verticalResolution = 11811;
    info_header_.colorTableEntries = 0;
    info_header_.importantColors = 0x1000000;

    ofstream out(file, ios::binary);
    out.write((char *) &file_header_, 14);
    out.write((char *) &info_header_, 40);

    const int w = image.GetWidth();
    const int h = image.GetHeight();
    const int padding = GetBMPStride(w);
    std::vector<char> buff(padding);

    for (int y = h - 1; y >= 0; --y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < w; ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        out.write(buff.data(), padding);
    }

    return out.good();
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    BitmapFileHeader file_header_;
    BitmapInfoHeader info_header_;
    ifstream ifs(file, ios::binary);

    ifs.read((char *) &file_header_, 14);
    ifs.read((char *) &info_header_, 40);

    int w, h, padding;
    w = info_header_.width;
    h = info_header_.height;
    padding = GetBMPStride(w);
    Image result(w, h, Color::Black());

    std::vector<char> buff(padding);
    for (int y = h - 1; y >= 0; --y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), padding);

        for (int x = 0; x < w; ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }
    return result;
}

}  // namespace img_lib