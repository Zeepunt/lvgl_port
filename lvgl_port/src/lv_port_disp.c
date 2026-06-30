/*
 * lv_port_disp.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2023-2026 Zeepunt
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "lvgl.h"
#include "lv_port.h"

#ifndef MY_DISP_HOR_RES
#define MY_DISP_HOR_RES 240
#endif

#ifndef MY_DISP_VER_RES
#define MY_DISP_VER_RES 320
#endif

static lv_display_t *s_disp = NULL;

void lv_port_disp_init(void)
{
    s_disp = lv_sdl_window_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    if (s_disp == NULL) {
        printf("sdl window crate failed.\n");
    }
}
