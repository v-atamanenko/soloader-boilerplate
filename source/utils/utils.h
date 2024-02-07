/*
 * Copyright (C) 2021      Rinnegatamante
 * Copyright (C) 2022-2024 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file  utils.h
 * @brief Common helper utilities.
 */

#ifndef SOLOADER_UTILS_H
#define SOLOADER_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Get Unix timestamp in milliseconds.
 *
 * @return Number of milliseconds that have elapsed since January 1, 1970.
 */
uint64_t current_timestamp_ms();

/**
 * Create a copy of a file.
 *
 * If the file specified by `destination` already exists, it will be
 * overwritten. If a parent directory or directories of the file specified by
 * `destination` do not exist, they will be created automatically.
 *
 * @warning The function will fail if the size of the source file specified by
 *          `path` exceeds the amount of free memory available.
 *
 * @param[in] path        Full path of the source file.
 * @param[in] destination Full path of the destination file.
 *
 * @return `true` on success, `false` otherwise.
 */
bool file_copy(const char * path, const char * destination);

/**
 * Check whether a file exists.
 *
 * @param path Full path of the file to look for.
 *
 * @return `true` if file exists, `false` otherwise.
 */
bool file_exists(const char * path);

/**
 * Load file contents into memory.
 *
 * @param[in]  path   Full path of the source file.
 * @param[out] buffer Output buffer address, allocated by the function. Must be
 *                    freed by the caller if the function returns `true`.
 * @param[out] size   Output buffer size.
 *
 * @return `true` on success, `false` otherwise.
 */
bool file_load(const char * path, uint8_t ** buffer, size_t * size);

/**
 * Create directories leading to file.
 *
 * @param[in] path Full path of the target file.
 * @param[in] mode Permissions to set for new directories (if any).
 *
 * @return `true` on success, `false` otherwise.
 */
bool file_mkpath(const char * path, mode_t mode);

/**
 * Save buffer contents into a file.
 *
 * @param[in] path   Full path of the target file.
 * @param[in] buffer Buffer containing data to save.
 * @param[in] size   Size of the buffer (in bytes).
 *
 * @return `true` on success, `false` otherwise.
 */
bool file_save(const char * path, const uint8_t * buffer, size_t size);

/**
 * Get the size of a file in bytes
 *
 * @param[in] path Full path of the target file.
 *
 * @return File size in bytes or (size_t)-1 in case of a failure.
 */
size_t file_size(const char * path);

/**
 * Get SHA1 hash of file contents.
 *
 * @param[in] path Full path of the source file.
 *
 * @return 40-char long null-terminated string containing SHA1 hash. Can be
 *         NULL in case of an error. Must be freed by the caller.
 */
char * file_sha1sum(const char * path);

/**
 * Check whether specified path is a directory.
 *
 * @param[in] path Target path.
 *
 * @return `true` if path is a directory, `false` otherwise.
 */
bool is_dir(const char * path);

/**
 * Check whether system module is loaded.
 *
 * @param[in] name Name of the system module to look for.
 *
 * @return `true` if the module is loaded, `false` otherwise.
 */
bool module_loaded(const char * name);

/**
 * Do nothing, return 0. Useful for stubbing.
 * @return 0
 */
int ret0(void);

/**
 * Do nothing, return 1. Useful for stubbing.
 * @return 1
 */
int ret1(void);

/**
 * Do nothing, return -1. Useful for stubbing.
 * @return -1
 */
int retminus1(void);

/**
 * Replace all occurrences of a substring in a string.
 *
 * @param[out] str         Target string. Will be extended using `realloc()` if
 *                         the resulting string is longer than the initial one.
 * @param[in]  needle      Substring to look for.
 * @param[in]  replacement Replacement.
 */
void str_replace(char ** str, const char * needle, const char * replacement);

/**
 * Remove all occurrences of a substring from a string.
 *
 * @param[out] str    Target string.
 * @param[in]  needle Substring to look for.
 */
void str_remove(char * str, const char * needle);

/**
 * Check whether a string starts with a substring.
 *
 * @param[in] str    Target string.
 * @param[in] prefix Substring to look for.
 *
 * @return `true` if the string starts with the substring, `false` otherwise.
 */
bool str_starts_with(const char * str, const char * prefix);

/**
 * Check whether a string ends with a substring.
 *
 * @param[in] str    Target string.
 * @param[in] suffix Substring to look for.
 *
 * @return `true` if the string ends with the substring, `false` otherwise.
 */
bool str_ends_with(const char * str, const char * suffix);

/**
 * Get SHA1 hash of a string or byte array.
 *
 * @param[in] str  Source string or byte array.
 * @param[in] size Length of the source string or byte array. If `0` is
 *                 specified, `str` is treated as a null-terminated string.
 *
 * @return 40-char long null-terminated string containing SHA1 hash. Can be
 *         NULL in case of an error. Must be freed by the caller.
 */
char * str_sha1sum(const char * str, size_t size);

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_UTILS_H
