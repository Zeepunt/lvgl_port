/*
 * @Author       : Zeepunt
 * @Date         : 2023-07-09
 * @LastEditTime : 2023-10-21
 *  
 * Gitee : https://gitee.com/zeepunt
 * Github: https://github.com/zeepunt
 *  
 * Copyright (c) 2023 by Zeepunt, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <lvgl_port/lv_port.h>
#include <lvgl_port/sdl/sdl.h>

#ifndef MY_DISP_HOR_RES
    #define MY_DISP_HOR_RES SDL_HOR_RES
#endif

#ifndef MY_DISP_VER_RES
    #define MY_DISP_VER_RES SDL_VER_RES
#endif

static lv_disp_t *s_disp = NULL;
static lv_disp_drv_t s_disp_drv;
static lv_disp_draw_buf_t s_disp_draw_buf;

static lv_color_t *s_disp_buf_1 = NULL;
static lv_color_t *s_disp_buf_2 = NULL;

static void disp_init(void)
{
    sdl_init();
}

void lv_port_disp_init(void)
{
    disp_init();

    uint32_t buf_size = sizeof(lv_color_t) * MY_DISP_HOR_RES * MY_DISP_VER_RES;

    s_disp_buf_1 = malloc(buf_size);
    if (NULL == s_disp_buf_1) {
        printf("disp buf 1 malloc fail.\n");
    }
    s_disp_buf_2 = malloc(buf_size);
    lv_disp_draw_buf_init(&s_disp_draw_buf, s_disp_buf_1, s_disp_buf_2, buf_size);     

    lv_disp_drv_init(&s_disp_drv);
    s_disp_drv.draw_buf = &s_disp_draw_buf;

    s_disp_drv.hor_res = MY_DISP_HOR_RES;
    s_disp_drv.ver_res = MY_DISP_VER_RES;

    s_disp_drv.flush_cb = sdl_display_flush;

    s_disp = lv_disp_drv_register(&s_disp_drv);
}
