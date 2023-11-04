# GAnyAndroid
[中文](README-zh.md)

Provide GAny support for Android, making it easy to load and call native code through GAny, and even support Lua extensions by loading gx script.

## Build and Use
### 1. Join your module
Due to the limitations of the Android system on dlopen, it is necessary to dynamically link your module to the project. The specific steps are as follows:
1. Add the source code or library files of the modules you need to load through G Any to the project.
2. Link your module for `gany-droid` in `CMakeList.txt`.
3. Introduce your module in `src/gany-droid/src/import_modules.cpp` as prompted by the code in macro `ENABLE_EXAMPLE`.
### 2. Build
Create a new build directory and enter it. Execute the following command (if cmake is not installed, please install it first, cmake version>=20):
```shell
cmake \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=$ABI \
    -DANDROID_PLATFORM=android-$MINSDKVERSION \
    -G "MinGW Makefiles" \  # Only specify toolchain when using Windows, recommend using Ninja if conditions permit
    ..
cmake --build . --target gany-droid
```
Description of parameters:[NDK Development Guide](https://developer.android.com/ndk/guides/cmake)
### 3. Deploy
1. Copy the files from `src/java` to your Android project
2. After the construction is completed, the generated library files will be organized into `bin/lib`. If your module is compiled as a dynamic library, please copy your library files and `libgany-droid.so` together to the specified `libs/ABI` directory of your Android project.

## Example
Path: `example/AndroidExample`, After the `gany-droid` is built, libs will be automatically copied to the sample project.


## License
`GAnyAndroid` is licensed under the [MIT License](LICENSE.txt).
