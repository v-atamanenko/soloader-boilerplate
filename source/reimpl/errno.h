/*
 * reimpl/errno.h
 *
 * Translation of newlib errno to bionic errno.
 *
 * Copyright (C) 2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_ERRNO_H
#define SOLOADER_ERRNO_H

#include <stddef.h>

int * __errno_soloader(void);
char * strerror_soloader(int error_number);
int strerror_r_soloader(int error_number, char* buf, size_t buf_len);

#endif // SOLOADER_ERRNO_H
