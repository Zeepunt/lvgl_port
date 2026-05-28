## 当前版本

[LVGL - 8.4.0](https://github.com/lvgl/lvgl/releases/tag/v8.4.0)。

## 平台编译

依赖项：

```shell
sudo apt install cmake
sudo apt install ninja-build
```

### 1、Linux

以 Ubuntu 为例

- 如果使用的是 SDL

    ```shell
    # 安装 SDL2
    sudo apt-get install aptitude
    sudo aptitude install libsdl2-dev
    
    # 确认已安装的 SDL2 版本 (当前所使用的版本为 2.0.20)
    sudo apt install aptitude
    aptitude show libsdl2-dev
    
    # 显示内容如下
    Package: libsdl2-dev
    Version: 2.0.20+dfsg-2ubuntu1.22.04.1
    State: installed
    ...
    ```

    编译和运行

    ```shell
    # 配置
    cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Debug
    
    # 编译
    cmake --build build
    
    # 运行
    ./build/lvgl_simulator
    ```

- 如果使用的是 Framebuffer

    ```shell
    # TODO
    ```

### 2、RTOS