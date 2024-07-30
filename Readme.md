## Сборка под Linux

Для запуска должена быть установлена LibJpeg:
    https://libjpeg.sourceforge.net/

Вот пример конфигурирования для Release и Debug:
```
# В из папки ImgConverter

# mkdir -p build-release 
# cd build-release
# cmake .. -DCMAKE_BUILD_TYPE=Release
# cmake --build .

# mkdir -p build-debug
# cd build-debug
# cmake .. -DCMAKE_BUILD_TYPE=Debug
# cmake --build .

```
## Запуск
Поддерживает форматы .BMP, .JPEG, .PPM

В папке `build` выполнить команду
```
./bin/imgconv <file-convert-from> <file-convert-to>
```
