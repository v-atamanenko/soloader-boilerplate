/*
 * utils/glutil.h
 *
 * OpenGL API initializer, related functions.
 *
 * Copyright (C) 2021      Andy Nguyen
 * Copyright (C) 2021      Rinnegatamante
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_GLUTIL_H
#define SOLOADER_GLUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vitaGL.h>

void gl_preload();
void gl_init();
void gl_swap();

#ifdef USE_CG_SHADERS
void glShaderSourceHook(GLuint shader, GLsizei count, const GLchar **string, const GLint *_length);
void glCompileShaderHook(GLuint shader);
#endif



#ifdef __cplusplus
};
#endif

#endif // SOLOADER_GLUTIL_H
