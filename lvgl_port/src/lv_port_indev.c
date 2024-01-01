/*
 * @Author       : Zeepunt
 * @Date         : 2023-12-30
 * @LastEditTime : 2023-12-30
 *  
 * Gitee : https://gitee.com/zeepunt
 * Github: https://github.com/zeepunt
 *  
 * Copyright (c) 2023 by Zeepunt, All Rights Reserved. 
 */
#include <lvgl_port/lv_port.h>
#include <lvgl_port/sdl/sdl.h>

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