/*
 * reimpl/ioctl.h
 *
 * Implementation for the ioctl() function
 *
 * Copyright (C) 2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_IOCTL_H
#define SOLOADER_IOCTL_H

#ifdef __cplusplus
extern "C" {
#endif

int ioctl_soloader(int fildes, int request, ... /* arg */);

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_IOCTL_H
