/*
 * utils/utils.c
 *
 * Common helper utilities.
 *
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "utils/utils.h"
#include "logger.h"

#include <psp2/io/stat.h>
#include <psp2/ctrl.h>

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <falso_jni/FalsoJNI.h>
#include <sha1/sha1.h>

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "bugprone-reserved-identifier"

int ret0(void) {
    return 0;
}

int ret1(void) {
    return 1;
}

int retminus1(void) {
    return -1;
}

SceUID _vshKernelSearchModuleByName(const char *, int *);

bool module_loaded(const char * name) {
    int search_unk[2];
    return _vshKernelSearchModuleByName(name, search_unk) >= 0;
}

bool file_exists(const char *path) {
    SceIoStat stat;
    return sceIoGetstat(path, &stat) >= 0;
}

bool string_ends_with(const char * str, const char * suffix) {
    int str_len = (int)strlen(str);
    int suffix_len = (int)strlen(suffix);

    return
            (str_len >= suffix_len) &&
            (0 == strcmp(str + (str_len-suffix_len), suffix));
}

uint64_t current_timestamp_ms() {
    struct timeval te;
    gettimeofday(&te, NULL);
    return (te.tv_sec*1000LL + te.tv_usec/1000);
}

void str_remove(char *str, const char *sub) {
    char *p, *q, *r;

    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
}

void str_replace(char *target, const char *needle, const char *replacement) {
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        tmp = p + needle_len;
    }

    strcpy(target, buffer);
}

void str_prepend(char* s, const char* t) {
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

bool is_dir(char* path) {
    DIR* filetest = opendir(path);
    if (filetest != NULL) {
        closedir(filetest);
        return true;
    }
    return false;
}

char * get_string_sha1(uint8_t* buf, long size) {
    uint8_t sha1[20];
    SHA1_CTX ctx;
    sha1_init(&ctx);
    sha1_update(&ctx, (uint8_t *)buf, size);
    sha1_final(&ctx, (uint8_t *)sha1);

    char hash[42];
    memset(hash, 0, sizeof(hash));

    int i;
    for (i = 0; i < 20; i++) {
        char string[4];
        sprintf(string, "%02X", sha1[i]);
        strcat(hash, string);
    }

    hash[41] = '\0';
    return strdup(hash);
}

char * get_file_sha1(const char* path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    void *buf = malloc(size);
    fread(buf, 1, size, f);
    fclose(f);

    char * ret = get_string_sha1(buf, size);
    free(buf);
    return ret;
}

int mkpath(char* file_path, mode_t mode) {
    assert(file_path && *file_path);
    for (char* p = strchr(file_path + 1, '/'); p; p = strchr(p + 1, '/')) {
        *p = '\0';
        if (mkdir(file_path, mode) == -1) {
            if (errno != EEXIST) {
                *p = '/';
                return -1;
            }
        }
        *p = '/';
    }
    return 0;
}

void file_save(const char* path, const uint8_t * buffer, size_t size) {
    FILE * f = fopen(path, "wb");
    if (f) {
        fwrite(buffer, size, 1, f);
        fclose(f);
    }
}

void cp(const char * src, const char * dst) {
    if (!file_exists(src)) return;

    char * mkpath_path = strdup(dst);
    mkpath(mkpath_path, 0755);
    free(mkpath_path);

    FILE *f = fopen(src, "rb");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    void *buf = malloc(size);
    fread(buf, 1, size, f);
    fclose(f);
    file_save(dst, buf, size);
    free(buf);
}
