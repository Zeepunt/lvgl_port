file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.c src/*.cpp)

add_library(lvgl_port OBJECT ${sources})
target_include_directories(lvgl_port PUBLIC include)
target_link_libraries(lvgl_port PUBLIC lvgl::lvgl)