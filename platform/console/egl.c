//
// Created by droc101 on 6/19/25.
//

#include "egl.h"
#include <stdio.h>
#include <EGL/egl.h>

EGLDisplay display;
EGLSurface surface;
EGLContext context;
EGLConfig config;

bool eglInit(ivec2 viewport)
{
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY)
    {
        fprintf(stderr, "Failed to get EGL display\n");
        return false;
    }

    if (!eglInitialize(display, NULL, NULL))
    {
        fprintf(stderr, "Failed to initialize EGL\n");
        return false;
    }

    const EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_SAMPLES, 0,
        EGL_NONE
    };

    EGLint numConfigs;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0)
    {
        fprintf(stderr, "Failed to choose EGL config\n");
        return false;
    }
    const EGLint pbufferAttribs[] = {
        EGL_WIDTH, viewport[0],
        EGL_HEIGHT, viewport[1],
        EGL_NONE,
    };
    surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    if (surface == EGL_NO_SURFACE)
    {
        fprintf(stderr, "Failed to create EGL Pbuffer surface\n");
        return false;
    }
    if (!eglBindAPI(EGL_OPENGL_ES_API))
    {
        fprintf(stderr, "Failed to bind OpenGL ES API\n");
        return false;
    }
    const EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        fprintf(stderr, "Failed to create EGL context\n");
        return false;
    }
    if (!eglMakeCurrent(display, surface, surface, context))
    {
        fprintf(stderr, "Failed to make EGL context current\n");
        return false;
    }

    return true;
}

void eglSwap()
{
    eglSwapBuffers(display, surface);
}

void eglCleanup()
{
    eglDestroySurface(display, surface);
    eglDestroyContext(display, context);
    eglTerminate(display);
}

bool eglResize(const ivec2 viewport)
{
    eglDestroySurface(display, surface);
    const EGLint pbufferAttribs[] = {
        EGL_WIDTH, viewport[0],
        EGL_HEIGHT, viewport[1],
        EGL_NONE,
    };
    surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    if (surface == EGL_NO_SURFACE)
    {
        fprintf(stderr, "Failed to create EGL Pbuffer surface\n");
        return false;
    }
    if (!eglMakeCurrent(display, surface, surface, context))
    {
        fprintf(stderr, "Failed to make EGL context current\n");
        return false;
    }

    return true;
}
