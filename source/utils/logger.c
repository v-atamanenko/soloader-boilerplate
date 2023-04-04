/*
 * utils/logger.c
 *
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "utils/logger.h"

#include <psp2/kernel/clib.h>
#include <psp2/kernel/threadmgr.h>

#define COLOR_RED     "\x1B[31m"
#define COLOR_ORANGE  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"

#define COLOR_END     "\033[0m"

static SceKernelLwMutexWork _log_mutex;
static volatile short int _log_mutex_inited = 0;

static char buffer_a[2048];
static char buffer_b[2048];

#define LOG_LOCK \
    if (!_log_mutex_inited) { \
        int ret = sceKernelCreateLwMutex(&_log_mutex, "log_lock", 0, 0, NULL); \
        if (ret < 0) { \
            sceClibPrintf("Error: failed to create log mutex: 0x%x\n", ret); \
            return; \
        } \
        _log_mutex_inited = 1; \
    } \
    sceKernelLockLwMutex(&_log_mutex, 1, NULL);

#define LOG_UNLOCK \
    if (_log_mutex_inited) { \
        sceKernelUnlockLwMutex(&_log_mutex, 1); \
    }

#define LOG_PRINT \
    va_list list; \
    va_start(list, fmt); \
    sceClibVsnprintf(buffer_b, sizeof(buffer_b), buffer_a, list); \
    va_end(list); \
    sceClibPrintf(buffer_b);

void _log_info(const char *fi, int li, const char *fn, const char* fmt, ...) {
#ifdef DEBUG_SOLOADER
    LOG_LOCK

    sceClibSnprintf(buffer_a, sizeof(buffer_a), "%s[INFO] %s%s\n",
                    COLOR_BLUE, fmt, COLOR_END);

    LOG_PRINT
    LOG_UNLOCK
#endif
}

void _log_warn(const char *fi, int li, const char *fn, const char* fmt, ...) {
#ifdef DEBUG_SOLOADER
    LOG_LOCK

    sceClibSnprintf(buffer_a, sizeof(buffer_a), "%s[WARN][%s:%d][%s] %s%s\n",
                    COLOR_ORANGE, fi, li, fn, fmt, COLOR_END);

    LOG_PRINT
    LOG_UNLOCK
#endif
}

void _log_debug(const char *fi, int li, const char *fn, const char* fmt, ...) {
#ifdef DEBUG_SOLOADER
    LOG_LOCK

    sceClibSnprintf(buffer_a, sizeof(buffer_a), "[DBG][%s:%d][%s] %s\n",
                    fi, li, fn, fmt);

    LOG_PRINT
    LOG_UNLOCK
#endif
}

void _log_error(const char *fi, int li, const char *fn, const char* fmt, ...) {
    LOG_LOCK

    sceClibSnprintf(buffer_a, sizeof(buffer_a), "%s[ERROR][%s:%d][%s] %s%s\n",
                    COLOR_RED, fi, li, fn, fmt, COLOR_END);

    LOG_PRINT
    LOG_UNLOCK
}
