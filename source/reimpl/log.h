/*
 * Copyright (C) 2009      The Android Open Source Project
 * Copyright (C) 2021      Andy Nguyen
 * Copyright (C) 2022      Rinnegatamante
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file  log.h
 * @brief Implementations for different Android logging functions.
 */

#ifndef SOLOADER_LOG_H
#define SOLOADER_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

/**
 * Android log priority values, in increasing order of priority.
 */
typedef enum android_LogPriority {
    /** For internal use only.  */
    ANDROID_LOG_UNKNOWN = 0,
    /** The default priority, for internal use only.  */
    ANDROID_LOG_DEFAULT, /* only for SetMinPriority() */
    /** Verbose logging. Should typically be disabled for a release apk. */
    ANDROID_LOG_VERBOSE,
    /** Debug logging. Should typically be disabled for a release apk. */
    ANDROID_LOG_DEBUG,
    /** Informational logging. Should typically be disabled for a release apk. */
    ANDROID_LOG_INFO,
    /** Warning logging. For use with recoverable failures. */
    ANDROID_LOG_WARN,
    /** Error logging. For use with unrecoverable failures. */
    ANDROID_LOG_ERROR,
    /** Fatal logging. For use when aborting. */
    ANDROID_LOG_FATAL,
    /** For internal use only.  */
    ANDROID_LOG_SILENT, /* only for SetMinPriority(); must be last */
} android_LogPriority;

/**
 * Writes the constant string `text` to the log, with priority `prio` and tag
 * `tag`.
 */
int __android_log_write(int prio, const char *tag, const char *text);

/**
 * Writes a formatted string to the log, with priority `prio` and tag `tag`.
 * The details of formatting are the same as for
 * [printf(3)](http://man7.org/linux/man-pages/man3/printf.3.html).
 */
int __android_log_print(int prio, const char *tag, const char *fmt, ...)
    __attribute__((__format__(printf, 3, 4)));

/**
 * Equivalent to `__android_log_print`, but taking a `va_list`.
 * (If `__android_log_print` is like `printf`, this is like `vprintf`.)
 */
int __android_log_vprint(int prio, const char *tag, const char *fmt, va_list ap)
    __attribute__((__format__(printf, 3, 0)));

/**
 * Writes an assertion failure to the log (as `ANDROID_LOG_FATAL`) and to
 * stderr, before calling
 * [abort(3)](http://man7.org/linux/man-pages/man3/abort.3.html).
 *
 * If `fmt` is non-null, `cond` is unused. If `fmt` is null, the string
 * `Assertion failed: %s` is used with `cond` as the string argument.
 * If both `fmt` and `cond` are null, a default string is provided.
 *
 * Most callers should use
 * [assert(3)](http://man7.org/linux/man-pages/man3/assert.3.html) from
 * `&lt;assert.h&gt;` instead, or the `__assert` and `__assert2` functions
 * provided by bionic if more control is needed. They support automatically
 * including the source filename and line number more conveniently than this
 * function.
 */
void __android_log_assert(const char* cond, const char* tag, const char* fmt, ...)
__attribute__((__noreturn__)) __attribute__((__format__(printf, 3, 4)));

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_LOG_H
