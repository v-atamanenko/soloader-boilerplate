/*
 * reimpl/log.h
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

#ifndef SOLOADER_LOG_H
#define SOLOADER_LOG_H

#include <stdarg.h>

int android_log_write(int prio, const char *tag, const char *msg);

int android_log_print(int prio, const char *tag, const char *fmt, ...);

int android_log_vprint(int prio, const char *tag, const char *fmt, va_list lst);

#endif // SOLOADER_LOG_H
