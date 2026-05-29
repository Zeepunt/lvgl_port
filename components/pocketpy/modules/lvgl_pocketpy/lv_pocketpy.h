/*
 * lv_pocketpy.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __LV_POCKETPY_H__
#define __LV_POCKETPY_H__

#include <lvgl.h>
#include <pocketpy.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    lv_obj_t *lv_obj;
} lv_pocketpy_obj_t;

lv_obj_t *lv_pocketpy_py_to_lvgl(py_Ref py_obj);
bool lv_pocketpy_lvgl_to_py(lv_obj_t *lv_obj);

void lv_pocketpy_module_btn(py_Ref mod);
void lv_pocketpy_module(void);

#ifdef __cplusplus
}
#endif

#endif /* __LV_POCKETPY_H__ */
