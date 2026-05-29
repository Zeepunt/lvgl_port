/*
 * lv_pocketpy.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "lv_pocketpy.h"

static py_Type s_lvgl_obj_type;

static void priv_obj_delete_event_cb(lv_event_t *e)
{
    lv_pocketpy_obj_t *pkpy_obj = lv_event_get_user_data(e);

    if (pkpy_obj != NULL) {
        pkpy_obj->lv_obj = NULL;
    }
}

lv_obj_t *lv_pocketpy_py_to_lvgl(py_Ref py_obj)
{
    lv_pocketpy_obj_t *pkpy_obj = NULL;

    if (py_isnil(py_obj)) {
        TypeError("Expected LVGL object");
        return NULL;
    }

    if (!py_checktype(py_obj, s_lvgl_obj_type)) {
        TypeError("Expected LVGL object");
        return NULL;
    }

    pkpy_obj = (lv_pocketpy_obj_t *)py_touserdata(py_obj);
    if (pkpy_obj == NULL) {
        RuntimeError("Expected LVGL object");
        return NULL;
    }

    if (pkpy_obj->lv_obj == NULL) {
        RuntimeError("Invalid LVGL object");
        return NULL;
    }

    return pkpy_obj->lv_obj;
}

bool lv_pocketpy_lvgl_to_py(lv_obj_t *lv_obj)
{
    lv_pocketpy_obj_t *pkpy_obj = NULL;

    if (lv_obj == NULL) {
        RuntimeError("Invalid LVGL object");
        return NULL;
    }

    pkpy_obj = (lv_pocketpy_obj_t *)py_newobject(py_retval(),
                                                 s_lvgl_obj_type,
                                                 0,
                                                 sizeof(lv_pocketpy_obj_t));
    if (pkpy_obj == NULL) {
        RuntimeError("Create create LVGL object failed");
        return false;
    }

    pkpy_obj->lv_obj = lv_obj;

    lv_obj_add_event_cb(lv_obj,
                        priv_obj_delete_event_cb,
                        LV_EVENT_DELETE,
                        pkpy_obj);

    return true;
}

void lv_pocketpy_module(void)
{
    py_Ref mod = py_newmodule("lvgl");

    s_lvgl_obj_type = py_newtype("lvgl_obj", tp_object, mod, NULL);

    lv_pocketpy_module_btn(mod);
}
