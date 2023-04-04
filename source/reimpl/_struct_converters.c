#define SC_INLINE static inline __attribute__((always_inline))

#define MUSL_O_WRONLY         01
#define MUSL_O_RDWR           02
#define MUSL_O_CREAT        0100
#define MUSL_O_EXCL         0200
#define MUSL_O_TRUNC       01000
#define MUSL_O_APPEND      02000
#define MUSL_O_NONBLOCK    04000

SC_INLINE int oflags_newlib_to_oflags_musl(int flags)
{
    int out = 0;
    if (flags & MUSL_O_RDWR)
        out |= O_RDWR;
    else if (flags & MUSL_O_WRONLY)
        out |= O_WRONLY;
    else
        out |= O_RDONLY;
    if (flags & MUSL_O_NONBLOCK)
        out |= O_NONBLOCK;
    if (flags & MUSL_O_APPEND)
        out |= O_APPEND;
    if (flags & MUSL_O_CREAT)
        out |= O_CREAT;
    if (flags & MUSL_O_TRUNC)
        out |= O_TRUNC;
    if (flags & MUSL_O_EXCL)
        out |= O_EXCL;
    return out;
}

SC_INLINE dirent64_bionic * dirent_newlib_to_dirent_bionic(struct dirent* dirent_newlib)
{
    dirent64_bionic * ret = malloc(sizeof(dirent64_bionic));
    strncpy(ret->d_name, dirent_newlib->d_name, sizeof(ret->d_name));
    ret->d_off = 0;
    ret->d_reclen = 0;
    ret->d_type = SCE_S_ISDIR(dirent_newlib->d_stat.st_mode) ? DT_DIR : DT_REG;
    return ret;
}

SC_INLINE void stat_newlib_to_stat_bionic(struct stat * src, stat64_bionic * dst)
{
    if (!src) return;
    if (!dst) dst = malloc(sizeof(stat64_bionic));

    dst->st_dev = src->st_dev;
    dst->st_ino = src->st_ino;
    dst->st_mode = src->st_mode;
    dst->st_nlink = src->st_nlink;
    dst->st_uid = src->st_uid;
    dst->st_gid = src->st_gid;
    dst->st_rdev = src->st_rdev;
    dst->st_size = src->st_size;
    dst->st_blksize = src->st_blksize;
    dst->st_blocks = src->st_blocks;
    dst->st_atime = src->st_atime;
    dst->st_atime_nsec = 0;
    dst->st_mtime = src->st_mtime;
    dst->st_mtime_nsec = 0;
    dst->st_ctime = src->st_ctime;
    dst->st_ctime_nsec = 0;
}
