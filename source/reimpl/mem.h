/*
 * Copyright (C) 2021      Andy Nguyen
 * Copyright (C) 2022      Rinnegatamante
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file  mem.h
 * @brief Implementations and wrappers for memory-related functions.
 */

#ifndef SOLOADER_MEM_H
#define SOLOADER_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

#define MAP_FAILED (void*)-1

void *sceClibMemclr(void *dst, size_t len);

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offs);

int munmap(void *addr, size_t length);

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_MEM_H
