/*
 * Copyright (C) 2023-2024 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file  errno.h
 * @brief Error code translation between newlib (Vita) and bionic (Android)
 *
 * Provides wrappers for errno-related functions that translate the underlying
 * error numbers and their corresponding descriptions into Bionic (Android)
 * format, so that the target can perform its error checking normally.
 */

#ifndef SOLOADER_ERRNO_H
#define SOLOADER_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

int *__errno_soloader(void);

char *strerror_soloader(int error_number);

int strerror_r_soloader(int error_number, char *buf, size_t buf_len);

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_ERRNO_H
