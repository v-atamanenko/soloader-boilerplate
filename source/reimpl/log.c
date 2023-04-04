/*
 * reimpl/log.c
 *
 * Implementations for different Android logging functions.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2022 Rinnegatamante
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "reimpl/log.h"

#include "utils/logger.h"

int android_log_write(int prio, const char *tag, const char *msg) {
    logv_debug("[ALOG][%i] %s: %s", prio, tag, msg);
    return 0;
}

int android_log_print(int prio, const char *tag, const char *fmt, ...) {
    va_list list;
    char msg[1024];

    va_start(list, fmt);
    sceClibVsnprintf(msg, sizeof(msg), fmt, list);
    va_end(list);

    logv_debug("[ALOG][%i] %s: %s", prio, tag, msg);
    return 0;
}

int android_log_vprint(int prio, const char *tag, const char *fmt, va_list lst) {
    char msg[1024];

    sceClibVsnprintf(msg, sizeof(msg), fmt, lst);

    logv_debug("[LOGV][%i] %s: %s", prio, tag, msg);
    return 0;
}
