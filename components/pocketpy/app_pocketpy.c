/*
 * app_pocketpy.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "pocketpy.h"
#include "app_pocketpy.h"

static char *priv_file_load(const char *file_name)
{
    int ret = 0;
    char *buf = NULL;

    struct stat st;
    FILE *fp = NULL;

    if (file_name == NULL) {
        goto exit;
    }

    ret = stat(file_name, &st);
    if (ret != 0) {
        printf("[%s:%d]file %s not exist\n", __FUNCTION__, __LINE__, file_name);
        goto exit;
    }
    printf("file %s size: %d\n", file_name, st.st_size);

    buf = malloc(st.st_size + 1);
    if (buf == NULL) {
        printf("[%s:%d]malloc failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }
    memset(buf, 0, st.st_size + 1);

    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        printf("[%s:%d]open %s failed\n", __FUNCTION__, __LINE__, file_name);
        goto exit;
    }
    fseek(fp, 0, SEEK_SET);

    if (st.st_size != fread(buf, 1, st.st_size, fp)) {
        printf("[%s:%d]read %s failed\n", __FUNCTION__, __LINE__, file_name);
        goto exit;
    }

    fclose(fp);
    fp = NULL;

    return buf;

exit:
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
    return NULL;
}

int app_pocketpy_init(void)
{
    py_initialize();

    return 0;
}

int app_pocketpy_run_py(const char *py_file)
{
    char *py_code = NULL;

    py_code = priv_file_load(py_file);
    if (py_code == NULL) {
        goto exit;
    }

    if (!py_exec(py_code, py_file, EXEC_MODE, NULL)) {
        py_printexc();
        goto exit;
    }

    return 0;

exit:
    return -1;
}
