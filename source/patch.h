/*
 * patch.h
 *
 * Patching some of the .so internal functions or bridging them to native for
 * better compatibility.
 *
 * Copyright (C) 2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_PATCH_H
#define SOLOADER_PATCH_H

#ifdef __cplusplus
extern "C" {
#endif

void so_patch();

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_PATCH_H
