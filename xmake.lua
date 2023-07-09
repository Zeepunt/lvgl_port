set_project("lvgl_port")
set_xmakever("2.7.8")
set_version("0.0.1", {build = "%Y%m%d%H%M"})

toolchain("target_toolchain")
    set_kind("standalone")
    set_bindir("/usr/bin")
toolchain_end()

set_languages("gnu99", "gnuxx11")
-- add_cflags("-g", "-DDEBUG")
add_cflags("-Os")

set_config("TARGET_DISPLAY", "sdl")
-- set_config("TARGET_DISPLAY", "framebuffer")

if get_config("TARGET_DISPLAY") == "sdl" then
    add_requires("SDL2", {system = true})
    add_defines("TARGET_DISPLAY_SDL")
    add_defines("LV_CONF_PATH=$(curdir)/port/sdl/lv_conf.h")
elseif get_config("TARGET_DISPLAY") == "framebuffer" then
    add_defines("TARGET_DISPLAY_FRAMEBUFFER")
    add_defines("LV_CONF_PATH=$(curdir)/port/framebuffer/lv_conf.h")
end

target("lvgl")
    set_kind("static")
    add_includedirs("lvgl", {public = true})
    add_includedirs("lvgl/src", {public = true})
    add_files("lvgl/src/**.c")

target("lvgl_port")
    set_kind("binary")
    add_deps("lvgl")
    add_links("pthread", "SDL2")
    add_includedirs("port")
    add_files("port/*.c")
if get_config("TARGET_DISPLAY") == "sdl" then
    add_includedirs("port/sdl")
    add_files("port/sdl/*.c")
elseif get_config("TARGET_DISPLAY") == "framebuffer" then
    add_includedirs("port/framebuffer")
    add_files("port/framebuffer/*.c")
end
    add_files("main.c")
    set_toolchains("target_toolchain")