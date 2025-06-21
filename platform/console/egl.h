//
// Created by droc101 on 6/19/25.
//

#ifndef EGL_H
#define EGL_H

#include <stdbool.h>
#include <cglm/cglm.h>

bool eglInit(ivec2 viewport);

void eglSwap();

void eglCleanup();

bool eglResize(const ivec2 viewport);

#endif //EGL_H
