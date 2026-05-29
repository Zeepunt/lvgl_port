/*
 * app_pocketpy.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __APP_POCKETPY_H__
#define __APP_POCKETPY_H__

#ifdef __cplusplus
extern "C" {
#endif

int app_pocketpy_init(void);

int app_pocketpy_run_py(const char *py_file);

#ifdef __cplusplus
}
#endif

#endif /* __APP_POCKETPY_H__ */
