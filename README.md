# 当前版本

[LVGL - 8.4.0](https://github.com/lvgl/lvgl/releases/tag/v8.4.0)。

# 平台编译

依赖项：

```shell
sudo apt install cmake
sudo apt install ninja-build
```

## 1、Linux

### Ubuntu

Ubuntu 版本：

```shell
$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 22.04.5 LTS
Release:        22.04
Codename:       jammy
```

#### 使用的是 SDL2

安装 SDL2

```shell
$ sudo apt install libsdl2-dev
```

编译和运行

```shell
# 配置
$ cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Debug

# 编译
$ cmake --build build

# 运行
$ ./build/lvgl_simulator
```

## 2、Windows

### MSYS2

MSYS2 版本：

```shell
# msys2-runtime 3.6.9
$ uname -a
MSYS_NT-10.0-19045 DESKTOP-1FVGBS5 3.6.9-01d6c708.x86_64 2026-05-24 10:45 UTC x86_64 Msys
```

> 1. 终端使用的是 `MSYS2 MINGW64`。
> 2. CMake 和 Ninja 都是使用 Windows 安装包，不是使用 pacman 命令安装的。

#### 使用的是 SDL2

安装 SDL2

```shell
$ pacman -S mingw-w64-x86_64-SDL2
```

编译和运行

```shell
# 配置
$ cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Debug

# 编译
$ cmake --build build

# 运行
$ ./build/lvgl_simulator
```
