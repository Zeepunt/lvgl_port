## 当前版本

LVGL - 8.3

- 分支为：`release/v8.3`。

- 所在节点为 `SHA-1: 1c5d4b02d725167f6903f2d4574ed5b5c1146194`：Merge branch 'demo-close' into release/v8.3。

## 平台编译

### 1、Linux

以 Ubuntu 为例

- 如果使用的是 SDL

    ```shell
    sudo apt-get install aptitude
    # 安装 SDL2
    sudo aptitude install libsdl2-dev
    # 确认已安装的 SDL2 版本
    # Version: 2.0.20+dfsg-2ubuntu1.22.04.1
    # 当前所使用的版本为 2.0.20
    aptitude show libsdl2-dev
    ```
    
    编译和运行
    
    ```shell
    # 修改 lv_port 目录下的 xmake.lua 文件, 开启下面的配置
    set_config("TARGET_DISPLAY", "sdl")
    
    # 编译
    xmake
    # 运行
    xmake run
    ```

- 如果使用的是 Framebuffer

    // TODO

### 2、RTOS