/*
 * lv_sdl_conf.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2023-2026 Zeepunt
 */
#ifndef __LV_SDL_CONF_H__
#define __LV_SDL_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_HOR_RES         240
#define SDL_VER_RES         320

/* Scale window by this factor (useful when simulating small screens) */
#define SDL_ZOOM            2

/**
 * Used to test true double buffering with only address changing.
 * Use 2 draw buffers, bith with SDL_HOR_RES x SDL_VER_RES size
 */
#define SDL_DOUBLE_BUFFERED 0

#define SDL_INCLUDE_PATH    <SDL2/SDL.h>

/* Open two windows to test multi display support */
#define SDL_DUAL_DISPLAY    0

#define SDL_WINDOW_TITLE    "TFT Simulator"

#ifdef __cplusplus
}
#endif

#endif /* __LV_SDL_CONF_H__ */