cmake_minimum_required(VERSION 3.25)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(LV_CONF_PATH 
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl_port/include/lvgl_port/lv_conf.h 
    CACHE STRING "lvgl config file path" FORCE)

add_compile_definitions(LV_CONF_PATH=${CMAKE_CURRENT_SOURCE_DIR}/lvgl_port/include/lvgl_port/lv_conf.h)

project(lvgl_simulator LANGUAGES C CXX)

find_package(SDL2 REQUIRED)

add_subdirectory(lvgl)
add_subdirectory(lvgl_port)

add_executable(lvgl_simulator main.c)
target_link_libraries(lvgl_simulator pthread
                                     SDL2
                                     lvgl_port)
