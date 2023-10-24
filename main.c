/*
 * @Author       : Zeepunt
 * @Date         : 2023-07-08
 * @LastEditTime : 2023-10-21
 *
 * Gitee : https://gitee.com/zeepunt
 * Github: https://github.com/zeepunt
 *
 * Copyright (c) 2023 by Zeepunt, All Rights Reserved.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <lvgl.h>
#include <lvgl_port/lv_port.h>

static pthread_t s_lvgl_handler_tid;
static pthread_t s_lvgl_timer_tid;

static void *lvgl_handler_thread(void *arg)
{
    while (1) {
        /* 周期性调用 lv_timer_handler */
        lv_timer_handler();
        /* 官方建议是最少 5ms */
        usleep(5 * 1000);
    }
}

static void *lvgl_timer_thread(void *arg)
{
    while (1) {
        usleep(5 * 1000);
        /* tick 单位是 ms */
        lv_tick_inc(5);
    }
}

void lvgl_demo(void)
{
    lv_obj_t *obj = lv_obj_create(lv_scr_act());
	lv_obj_set_pos(obj, 0, 0);
	lv_obj_set_size(obj, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));

    lv_obj_t *label = lv_label_create(obj);
	lv_label_set_text(label, "Hello, LVGL.");
    lv_obj_center(label);
}

int main(int argc, char *argv[])
{
    int ret = -1;

    /* 初始化 lvgl */
    lv_init();

    /* 设备初始化 */
    lv_port_disp_init();

    ret = pthread_create(&s_lvgl_handler_tid,
                         NULL,
                         lvgl_handler_thread,
                         NULL);
    if (0 == ret) {
        pthread_detach(s_lvgl_handler_tid);
    } else {
        printf("lvgl_handler thread crate fail: %d.\n", ret);
    }

    ret = pthread_create(&s_lvgl_timer_tid,
                         NULL,
                         lvgl_timer_thread,
                         NULL);
    if (0 == ret) {
        pthread_detach(s_lvgl_timer_tid);
    } else {
        printf("lvgl_timer thread crate fail: %d.\n", ret);
    }

    lvgl_demo();

    while (1);

    ret = 0;
    return ret;
}
