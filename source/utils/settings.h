/*
 * utils/settings.h
 *
 * Loader settings that can be set via the companion app.
 *
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_SETTINGS_H
#define SOLOADER_SETTINGS_H
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int  setting_sampleSetting;
extern bool setting_sampleSetting2;

void settings_load();
void settings_save();
void settings_reset();

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_SETTINGS_H
