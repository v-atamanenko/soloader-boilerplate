/*
 * reimpl/ioctl.c
 *
 * Implementation for the ioctl() function
 *
 * Copyright (C) 2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "reimpl/ioctl.h"

#include "utils/logger.h"

int ioctl_soloader(int fildes, int request, ... /* arg */) {
	logv_error("ioctl: fd %i, request %i (0x%x)", fildes, request, request);
	return 0;
}
