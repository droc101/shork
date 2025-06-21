//
// Created by droc101 on 6/19/25.
//
#include "platform_sdl.h"
#include <SDL3/SDL.h>
#include "../../renderer.h"

SDL_GLContext ctx;
SDL_Window *window;

bool sdlInit(const char *flag)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("shork",
                                          640, 480,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_SURFACE_VSYNC_DISABLED);

    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetSwapInterval(0);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, GL_PROFILE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    ctx = SDL_GL_CreateContext(window);
    if (ctx == NULL)
    {
        SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
        return false;
    }

    ivec2 resolution;
    SDL_GetWindowSize(window, &resolution[0], &resolution[1]);
    if (!renderInit(resolution, flag, NULL, false))
    {
        SDL_GL_DestroyContext(ctx);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

void sdlSwap()
{
    SDL_GL_SwapWindow(window);
}

void sdlLoop()
{
    while (1)
    {
        glRenderFrame(sdlSwap);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                return;
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                const ivec2 newSize = {event.window.data1, event.window.data2};
                if (!renderResize(newSize, NULL))
                {
                    SDL_Log("Failed to resize render");
                    return;
                }
            }
        }
    }
}

void sdlCleanup()
{
    renderCleanup(NULL);
    SDL_GL_DestroyContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
