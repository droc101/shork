//
// Created by droc101 on 6/19/25.
//

#ifndef PLATFORM_SDL_H
#define PLATFORM_SDL_H
#include <stdbool.h>

#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 2
#define GL_PROFILE SDL_GL_CONTEXT_PROFILE_ES

bool sdlInit(const char *flag);

void sdlLoop();

void sdlCleanup();

#endif //PLATFORM_SDL_H
