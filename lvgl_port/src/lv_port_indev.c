/*
 * lv_port_indev.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2023-2026 Zeepunt
 */
#include "lvgl.h"
#include "lv_port.h"

static lv_indev_t *s_indev_mouse = NULL;
static lv_indev_t *s_indev_mousewheel = NULL;
static lv_indev_t *s_index_keypad = NULL;

void lv_port_indev_init(void)
{
    s_indev_mouse = lv_sdl_mouse_create();
    s_indev_mousewheel = lv_sdl_mousewheel_create();
    s_index_keypad = lv_sdl_keyboard_create();
}
