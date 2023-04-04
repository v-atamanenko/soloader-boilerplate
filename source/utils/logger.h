/*
 * utils/logger.h
 *
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_LOGGER_H
#define SOLOADER_LOGGER_H

#include <psp2/kernel/clib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Utility for quick verbose logging of crashing functions.
#define dbg sceClibPrintf("[DBG] [%s:%s:%d]\n", __FILE__, __func__, __LINE__)

#define logv_info(fmt, ...)   _log_info(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define logv_warn(fmt, ...)   _log_warn(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define logv_debug(fmt, ...) _log_debug(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define logv_error(fmt, ...) _log_error(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)

#define log_info(fmt)   _log_info(__FILE__, __LINE__, __func__, fmt)
#define log_warn(fmt)   _log_warn(__FILE__, __LINE__, __func__, fmt)
#define log_debug(fmt) _log_debug(__FILE__, __LINE__, __func__, fmt)
#define log_error(fmt) _log_error(__FILE__, __LINE__, __func__, fmt)

void _log_info(const char *fname, int lineno, const char *fxname, const char* fmt, ...);
void _log_warn(const char *fname, int lineno, const char *fxname, const char* fmt, ...);
void _log_debug(const char *fname, int lineno, const char *fxname, const char* fmt, ...);
void _log_error(const char *fname, int lineno, const char *fxname, const char* fmt, ...);

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_LOGGER_H
