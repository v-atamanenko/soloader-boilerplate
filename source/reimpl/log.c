/*
 * reimpl/log.c
 *
 * Implementations for different Android logging functions.
 *
 * Copyright (C) 2021      Andy Nguyen
 * Copyright (C) 2022      Rinnegatamante
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "reimpl/log.h"

#include "utils/logger.h"

#define print_common \
    switch (prio) { \
        case ANDROID_LOG_INFO: \
            logv_info("[ALOG][%s] %s", tag, text); \
            break; \
        case ANDROID_LOG_WARN: \
            logv_warn("[ALOG][%s] %s", tag, text); \
            break; \
        case ANDROID_LOG_ERROR: \
        case ANDROID_LOG_FATAL: \
            logv_error("[ALOG][%s] %s", tag, text); \
            break; \
        case ANDROID_LOG_UNKNOWN: \
        case ANDROID_LOG_DEFAULT: \
        case ANDROID_LOG_VERBOSE: \
        case ANDROID_LOG_DEBUG: \
        case ANDROID_LOG_SILENT: \
        default: \
            logv_debug("[ALOG][%s] %s", tag, text); \
            break; \
    }

int __android_log_write(int prio, const char* tag, const char* text) {
    print_common
    return 0;
}

int __android_log_print(int prio, const char* tag, const char* fmt, ...) {
    va_list list;
    char text[1024];

    va_start(list, fmt);
    sceClibVsnprintf(text, sizeof(text), fmt, list);
    va_end(list);

    print_common

    return 0;
}

int __android_log_vprint(int prio, const char* tag, const char* fmt, va_list ap) {
    char text[1024];

    sceClibVsnprintf(text, sizeof(text), fmt, ap);

    print_common

    return 0;
}
