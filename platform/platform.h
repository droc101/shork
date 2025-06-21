//
// Created by droc101 on 6/19/25.
//

#ifndef PLATFORM_H
#define PLATFORM_H
#include "console/platform_console.h"
#include "sdl/platform_sdl.h"

typedef bool (*platformInit)(const char*);
typedef void (*platformLoop)(void);
typedef void (*platformCleanup)(void);

typedef enum PlatformID
{
    PLATFORM_CONSOLE,
    PLATFORM_SDL,
    PLATFORM_MAX
} PlatformID;

typedef struct Platform
{
    PlatformID id;
    platformInit init;
    platformLoop loop;
    platformCleanup cleanup;
} Platform;

Platform platforms[PLATFORM_MAX] = {
    {
        PLATFORM_CONSOLE,
        consoleInit,
        consoleLoop,
        consoleCleanup,
    },
    {
        PLATFORM_SDL,
        sdlInit,
        sdlLoop,
        sdlCleanup,
    }
};

#endif //PLATFORM_H
