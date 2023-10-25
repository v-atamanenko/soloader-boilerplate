/*
 * utils/glutil.c
 *
 * OpenGL API initializer, related functions.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "utils/glutil.h"

#include "utils/utils.h"
#include "utils/dialog.h"
#include "utils/logger.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/io/stat.h>

#define GLSL_PATH DATA_PATH
#define GXP_PATH "app0:shaders"

void gl_preload() {
    if (!file_exists("ur0:/data/libshacccg.suprx")
        && !file_exists("ur0:/data/external/libshacccg.suprx")) {
        fatal_error("Error: libshacccg.suprx is not installed. Google \"ShaRKBR33D\" for quick installation.");
    }
}

void gl_init() {
    vglInitExtended(0, 960, 544, 6 * 1024 * 1024, SCE_GXM_MULTISAMPLE_4X);
}

void gl_swap() {
    vglSwapBuffers(GL_FALSE);
}

GLboolean skip_next_compile = GL_FALSE;
char next_shader_fname[128];

void load_shader(GLuint shader, const char * string, size_t length) {
    char* sha_name = get_string_sha1((uint8_t*)string, length);

    char gxp_path[128];
    snprintf(gxp_path, sizeof(gxp_path), DATA_PATH"gxp/%c%c/%s.gxp", sha_name[0], sha_name[1], sha_name);

    FILE *file = fopen(gxp_path, "rb");
    if (!file) {
        mkpath(gxp_path, 0777);
        glShaderSource(shader, 1, &string, &length);
        snprintf(next_shader_fname, sizeof(next_shader_fname), DATA_PATH"gxp/%c%c/%s.gxp", sha_name[0], sha_name[1], sha_name);
    } else {
        fseek(file, 0, SEEK_END);
        uint32_t size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char * shaderBuf = malloc(size + 1);
        fread(shaderBuf, 1, size, file);
        shaderBuf[size] = 0;
        fclose(file);

        glShaderBinary(1, &shader, 0, shaderBuf, (int32_t)size);

        if(shaderBuf) free(shaderBuf);
        skip_next_compile = GL_TRUE;
    }
    free(sha_name);
}

void glShaderSourceHook(GLuint shader, GLsizei count, const GLchar **string,
                        const GLint *_length) {
    if (!string) {
        log_error("Shader source string is NULL");
        return;
    } else if (!*string) {
        log_error("Shader source *string is NULL");
        return;
    }

    int total_length = 0;

    for (int i = 0; i < count; ++i) {
        if (!_length) {
            total_length += strlen(string[i]);
        } else {
            total_length += _length[i];
        }
    }

    char * str = malloc(total_length+1);
    int l = 0;

    for (int i = 0; i < count; ++i) {
        if (!_length) {
            memcpy(str + l, string[i], strlen(string[i]));
            l += strlen(string[i]);
        } else {
            memcpy(str + l, string[i], _length[i]);
            l += _length[i];
        }
    }
    str[total_length] = '\0';

    load_shader(shader, str, total_length);
    free(str);
}

void glCompileShaderHook(GLuint shader) {
    if (!skip_next_compile) {
        glCompileShader(shader);
        void *bin = vglMalloc(32 * 1024);
        GLsizei len;
        vglGetShaderBinary(shader, 32 * 1024, &len, bin);
        FILE *file = fopen(next_shader_fname, "wb");
        fwrite(bin, 1, len, file);
        fclose(file);
        vglFree(bin);
    }
    skip_next_compile = GL_FALSE;
}

EGLBoolean eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor) {
    logv_debug("eglInitialize(0x%x)", (int)dpy);

    gl_init();

    if (major) *major = 2;
    if (minor) *minor = 2;

    return EGL_TRUE;
}

EGLBoolean eglQuerySurface(EGLDisplay dpy, EGLSurface eglSurface, EGLint attribute, EGLint *value)
{
    EGLBoolean ret = EGL_TRUE;
    switch (attribute) {
        case EGL_CONFIG_ID:
            ret = 1;
            break;
        case EGL_WIDTH:
            *value = 960;
            break;
        case EGL_HEIGHT:
            *value = 544;
            break;
        case EGL_TEXTURE_FORMAT:
            *value = 2; // NoTexture = 0, RGB = 1, RGBA = 2
            break;
        case EGL_TEXTURE_TARGET:
            *value = 1;
            break;
        case EGL_SWAP_BEHAVIOR:
            ret = EGL_TRUE;
            *value = EGL_BUFFER_PRESERVED;
            break;
        case EGL_LARGEST_PBUFFER:
        case EGL_MIPMAP_TEXTURE:
            *value = EGL_FALSE;
            break;
        case EGL_MIPMAP_LEVEL:
            *value = 0;
            break;
        case EGL_MULTISAMPLE_RESOLVE:
            // ignored when creating the surface, return default
            *value = EGL_MULTISAMPLE_RESOLVE_DEFAULT;
            break;
        case EGL_HORIZONTAL_RESOLUTION:
        case EGL_VERTICAL_RESOLUTION:
            *value = 220 * EGL_DISPLAY_SCALING; // VITA DPI is 220
            break;
        case EGL_PIXEL_ASPECT_RATIO:
            // Please don't ask why * EGL_DISPLAY_SCALING, the document says it
            *value = 960 / 544 * EGL_DISPLAY_SCALING;
            break;
        case EGL_RENDER_BUFFER:
            *value = EGL_BACK_BUFFER;
            break;
        case EGL_VG_COLORSPACE:
            // ignored when creating the surface, return default
            *value = EGL_VG_COLORSPACE_sRGB;
            break;
        case EGL_VG_ALPHA_FORMAT:
            // ignored when creating the surface, return default
            *value = EGL_VG_ALPHA_FORMAT_NONPRE;
            break;
        case EGL_TIMESTAMPS_ANDROID:
            *value = EGL_FALSE;
            break;
        default:
            logv_error("eglQuerySurface %x  EGL_BAD_ATTRIBUTE", attribute);
            break;
    }

    return ret;
}


EGLBoolean eglGetConfigAttrib(EGLDisplay display,
                              EGLConfig config,
                              EGLint attribute,
                              EGLint * value) {
    switch (attribute) {
        case EGL_ALPHA_SIZE: {
            *value = 8;
            break;
        }
        case EGL_ALPHA_MASK_SIZE: {
            *value = 8;
            break;
        }
        case EGL_BIND_TO_TEXTURE_RGB: {
            *value = EGL_TRUE;
            break;
        }
        case EGL_BIND_TO_TEXTURE_RGBA: {
            *value = EGL_TRUE;
            break;
        }
        case EGL_BLUE_SIZE: {
            *value = 8;
            break;
        }
        case EGL_BUFFER_SIZE: {
            *value = 32;
            break;
        }
        case EGL_COLOR_BUFFER_TYPE: {
            *value = EGL_RGB_BUFFER;
            break;
        }
        case EGL_CONFIG_CAVEAT: {
            *value = EGL_NONE;
            break;
        }
        case EGL_CONFIG_ID: {
            *value = 1;
            break;
        }
        case EGL_CONFORMANT: {
            *value = 0;
            break;
        }
        case EGL_DEPTH_SIZE: {
            *value = 0;
            break;
        }
        case EGL_GREEN_SIZE: {
            *value = 0;
            break;
        }
        case EGL_LEVEL: {
            *value = 0;
            break;
        }
        case EGL_LUMINANCE_SIZE: {
            *value = 0;
            break;
        }
        case EGL_MAX_PBUFFER_WIDTH: {
            *value = 0;
            break;
        }
        case EGL_MAX_PBUFFER_HEIGHT: {
            *value = 0;
            break;
        }
        case EGL_MAX_PBUFFER_PIXELS: {
            *value = 0;
            break;
        }
        case EGL_MAX_SWAP_INTERVAL: {
            *value = 0;
            break;
        }
        case EGL_MIN_SWAP_INTERVAL: {
            *value = 0;
            break;
        }
        case EGL_NATIVE_RENDERABLE: {
            *value = 0;
            break;
        }
        case EGL_NATIVE_VISUAL_ID: {
            *value = 0;
            break;
        }
        case EGL_NATIVE_VISUAL_TYPE: {
            *value = 0;
            break;
        }
        case EGL_RED_SIZE: {
            *value = 0;
            break;
        }
        case EGL_RENDERABLE_TYPE: {
            *value = 0;
            break;
        }
        case EGL_SAMPLE_BUFFERS: {
            *value = 0;
            break;
        }
        case EGL_SAMPLES: {
            *value = 0;
            break;
        }
        case EGL_STENCIL_SIZE: {
            *value = 0;
            break;
        }
        case EGL_SURFACE_TYPE: {
            *value = 0;
            break;
        }
        case EGL_TRANSPARENT_TYPE: {
            *value = 0;
            break;
        }
        case EGL_TRANSPARENT_RED_VALUE: {
            *value = 0;
            break;
        }
        case EGL_TRANSPARENT_GREEN_VALUE: {
            *value = 0;
            break;
        }
        case EGL_TRANSPARENT_BLUE_VALUE: {
            *value = 0;
            break;
        }
        default:
            return EGL_FALSE;
    }
    return EGL_TRUE;
}

EGLBoolean eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config) {
    if (!num_config) {
        return EGL_BAD_PARAMETER;
    }

    if (!configs) {
        *num_config = 1;
        return EGL_TRUE;
    }

    *configs = strdup("1");
    *num_config = 1;

    return EGL_TRUE;
}

EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list) {
    return strdup("ctx");
}

EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, void * win, const EGLint *attrib_list) {
    return strdup("surface");
}

EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) {
    return EGL_TRUE;
}

EGLBoolean eglDestroyContext (EGLDisplay dpy, EGLContext ctx) {
    if (ctx) free(ctx);
    return EGL_TRUE;
}

EGLBoolean eglDestroySurface (EGLDisplay dpy, EGLSurface surface) {
    if (surface) free(surface);
    return EGL_TRUE;
}

EGLBoolean eglTerminate(EGLDisplay dpy) {
    return EGL_TRUE;
}
