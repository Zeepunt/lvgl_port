cmake_minimum_required(VERSION 3.22)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(lvgl_simulator LANGUAGES C CXX)

find_package(SDL2 REQUIRED)

add_subdirectory(lvgl)
add_subdirectory(lvgl_port)
add_subdirectory(components)

target_include_directories(lvgl
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl_port/inc
)

add_executable(lvgl_simulator
    main.c
)

target_link_libraries(lvgl_simulator
    pthread
    lvgl_port
    components
)
