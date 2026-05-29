/*
 * lv_pocketpy_btn.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "lv_pocketpy.h"

static bool priv_btn_create(int argc, py_Ref argv)
{
    lv_obj_t *parent = NULL;
    lv_obj_t *btn = NULL;

    if (argc == 1) {
        parent = lv_pocketpy_py_to_lvgl(py_arg(0));
    }

    btn = lv_btn_create((parent != NULL) ? parent : lv_scr_act());
    if (btn == NULL) {
        RuntimeError("lv_btn_create failed");
        return false;
    }

    return lv_pocketpy_lvgl_to_py(btn);
}

void lv_pocketpy_module_btn(py_Ref mod)
{
    py_bindfunc(mod, "btn_create", priv_btn_create);
}
