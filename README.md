## 当前版本

[LVGL - 8.3.8](https://github.com/lvgl/lvgl/releases/tag/v8.3.8)。

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
    
    # 确认已安装的 SDL2 版本
    # Version: 2.0.20+dfsg-2ubuntu1.22.04.1
    # 当前所使用的版本为 2.0.20
    aptitude show libsdl2-dev
    ```

    编译和运行

    ```shell
    # 编译
    python make.py
    
    #运行
    python make.py run
    ```

- 如果使用的是 Framebuffer

    ```shell
    # TODO
    ```

### 2、RTOS