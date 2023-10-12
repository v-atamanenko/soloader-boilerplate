/*
 * reimpl/io.c
 *
 * Wrappers and implementations for some of the IO functions.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2022 Rinnegatamante
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "reimpl/io.h"

#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <psp2/kernel/threadmgr.h>

#ifdef USE_SCELIBC_IO
#include <libc_bridge/libc_bridge.h>
#endif

#include "utils/logger.h"
#include "utils/utils.h"

// Includes the following inline utilities:
// int oflags_newlib_to_oflags_musl(int flags);
// dirent64_bionic * dirent_newlib_to_dirent_bionic(struct dirent* dirent_newlib);
// void stat_newlib_to_stat_bionic(struct stat * src, stat64_bionic * dst);
#include "_struct_converters.c"

FILE *fopen_soloader(char *fname, char *mode) {
    if (strcmp(fname, "/proc/cpuinfo") == 0) {
        return fopen_soloader("app0:/cpuinfo", mode);
    } else if (strcmp(fname, "/proc/meminfo") == 0) {
        return fopen_soloader("app0:/meminfo", mode);
    }

    #ifdef USE_SCELIBC_IO
        FILE* ret = sceLibcBridge_fopen(fname, mode);
    #else
        FILE* ret = fopen(fname, mode);
    #endif

    logv_debug("[io] fopen(%s, %s): 0x%x", fname, mode, ret);

    return ret;
}

int open_soloader(char *_fname, int flags) {
    if (strcmp(_fname, "/proc/cpuinfo") == 0) {
        return open_soloader("app0:/cpuinfo", flags);
    } else if (strcmp(_fname, "/proc/meminfo") == 0) {
        return open_soloader("app0:/meminfo", flags);
    }

    flags = oflags_newlib_to_oflags_musl(flags);
    int ret = open(_fname, flags);
    logv_debug("[io] open(%s, %x): %i", _fname, flags, ret);
    return ret;
}

int fstat_soloader(int fd, void *statbuf) {
    struct stat st;
    int res = fstat(fd, &st);
    if (res == 0)
        stat_newlib_to_stat_bionic(&st, statbuf);

    logv_debug("[io] fstat(fd#%i): %i", fd, res);
    return res;
}

int stat_soloader(char *_pathname, stat64_bionic *statbuf) {
    struct stat st;
    int res = stat(_pathname, &st);

    if (res == 0)
        stat_newlib_to_stat_bionic(&st, statbuf);

    logv_debug("[io] stat(%s): %i", _pathname, res);
    return res;
}

int fclose_soloader(FILE * f) {
    #ifdef USE_SCELIBC_IO
        int ret = sceLibcBridge_fclose(f);
    #else
        int ret = fclose(f);
    #endif

    logv_debug("[io] fclose(0x%x): %i", f, ret);
    return ret;
}

int close_soloader(int fd) {
    int ret = close(fd);
    logv_debug("[io] close(fd#%i): %i", fd, ret);
    return ret;
}

DIR* opendir_soloader(char* _pathname) {
    DIR* ret = opendir(_pathname);
    logv_debug("[io] opendir(\"%s\"): 0x%x", _pathname, ret);
    return ret;
}

struct dirent * readdir_soloader(DIR * dir) {
    struct dirent* ret = readdir(dir);
    log_debug("[io] readdir()");
    return ret;
}

int readdir_r_soloader(DIR *dirp, dirent64_bionic *entry, dirent64_bionic **result) {
    struct dirent dirent_tmp;
    struct dirent* pdirent_tmp;

    int ret = readdir_r(dirp, &dirent_tmp, &pdirent_tmp);

    if (ret == 0) {
        dirent64_bionic* entry_tmp = dirent_newlib_to_dirent_bionic(&dirent_tmp);
        memcpy(entry, entry_tmp, sizeof(dirent64_bionic));
        *result = (pdirent_tmp != NULL) ? entry : NULL;
        free(entry_tmp);
    }

    log_debug("[io] readdir_r()");
    return ret;
}

int closedir_soloader(DIR* dir) {
    int ret = closedir(dir);
    logv_debug("[io] closedir(0x%x): %i", dir, ret);
    return ret;
}

int fcntl_soloader(int fd, int cmd, ...) {
    logv_debug("[io] fcntl(fd#%i, cmd#%i)", fd, cmd);
    return 0;
}
