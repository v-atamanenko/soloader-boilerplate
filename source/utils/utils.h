/*
 * utils/utils.h
 *
 * Common helper utilities.
 *
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_UTILS_H
#define SOLOADER_UTILS_H

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

int ret0(void);

__attribute__((unused)) int ret1(void);

int retminus1(void);

bool file_exists(const char *path);

char * get_string_sha1(uint8_t* buf, long size);

char * get_file_sha1(const char* path);

bool module_loaded(const char * name);

bool string_ends_with(const char * str, const char * suffix);

/* Prepends t into s. Assumes s has enough space allocated
** for the combined string.
*/
void str_prepend(char* s, const char* t);

__attribute__((unused)) inline int string_starts_with(const char *pre,
                                                      const char *str) {
    char cp;
    char cs;

    if (!*pre)
        return 1;

    while ((cp = *pre++) && (cs = *str++))
    {
        if (cp != cs)
            return 0;
    }

    if (!cs)
        return 0;

    return 1;
}

uint64_t current_timestamp_ms();

void str_remove(char *str, const char *sub);

void str_replace(char *target, const char *needle, const char *replacement);

bool is_dir(char* path);

void file_save(const char* path, const uint8_t * buffer, size_t size);

/*
 * mkpath(): create all directories leading to file `file_path`
 * returns: 0 on success, -1 on fail (and sets errno)
 */
int mkpath(char* file_path, mode_t mode);

void cp(const char * src, const char * dst);

#endif // SOLOADER_UTILS_H
