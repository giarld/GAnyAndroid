# GAnyAndroid
[English](README.md)

为 Android 提供 GAny 支持, 方便通过 GAny 加载和调用 native 代码, 甚至通过装载 gx-script 来支持 lua 扩展.

## 构建与使用
### 1. 加入你的模块
因Android系统对dlopen的限制, 因此需要将你的模块动态链接到工程中, 具体步骤如下:
1. 将你需要通过GAny加载的模块的源码或库文件加入到工程中.
2. 在 `CMakeList.txt` 中为 `gany-droid` 链接你的模块.
3. 在 `src/gany-droid/src/import_modules.cpp` 中按照宏 `ENABLE_EXAMPLE` 内代码所提示的方式, 引入你的模块.
### 2. 构建
新建 build 目录并进入, 执行以下命令(如果没安装cmake, 请先安装, cmake version >= 20):
```shell
cmake \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=$ABI \
    -DANDROID_PLATFORM=android-$MINSDKVERSION \
    -G "MinGW Makefiles" \  # 仅当 Windows 时指定工具链, 有条件的情况下推荐使用Ninja
    ..
cmake --build . --target gany-droid
```
参数的说明: [NDK开发指南](https://developer.android.com/ndk/guides/cmake)
### 3. 部署
1. 将 `src/java` 中的文件复制到你的 Android 工程.
2. 构建完成后, 生成的库文件将整理到 `bin/lib` 中, 如果你的模块是编译为动态库, 请将你的库文件和 `libgany-droid.so` 一起复制到你的 Android 工程的指定 `libs/ABI` 目录中.

## 示例
位置: `example/AndroidExample`, 在 `gany-droid` 构建完后, 会自动复制 libs 到示例工程.

## 许可
`GAnyAndroid` 根据 [MIT许可证](LICENSE.txt) 授权。
