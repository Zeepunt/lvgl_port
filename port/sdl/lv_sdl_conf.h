/*
 * @Author       : Zeepunt
 * @Date         : 2023-07-08 22:51:28
 * @LastEditTime : 2023-07-09 12:15:11
 *  
 * Gitee : https://gitee.com/zeepunt
 * Github: https://github.com/zeepunt
 *  
 * Copyright (c) 2023 by Zeepunt, All Rights Reserved. 
 */
#ifndef __LV_SDL_CONF_H__
#define __LV_SDL_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_HOR_RES         720
#define SDL_VER_RES         480

/* Scale window by this factor (useful when simulating small screens) */
#define SDL_ZOOM            1

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