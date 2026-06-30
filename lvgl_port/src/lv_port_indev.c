/*
 * lv_port_indev.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2023-2026 Zeepunt
 */
#include "lv_port.h"
#include "sdl/sdl.h"

static lv_indev_drv_t s_indev_drv;
static lv_indev_t *s_mouse_indev;

void lv_port_indev_init(void)
{
    lv_indev_drv_init(&s_indev_drv);
    
    s_indev_drv.type = LV_INDEV_TYPE_POINTER;
    s_indev_drv.read_cb = sdl_mouse_read;

    s_mouse_indev = lv_indev_drv_register(&s_indev_drv);

    LV_IMG_DECLARE(mouse_cursor_icon);
    lv_obj_t * cursor_obj = NULL;
    cursor_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);
    lv_indev_set_cursor(s_mouse_indev, cursor_obj); // 设置光标
}