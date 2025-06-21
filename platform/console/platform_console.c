//
// Created by droc101 on 6/19/25.
//

#include "platform_console.h"
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <EGL/egl.h>
#include "ansi.h"
#include "egl.h"
#include "tty.h"
#include "../../renderer.h"

ivec2 consoleSize = {0, 0};
bool stop = false;

void consoleRenderFrame()
{
    glRenderFrame(eglSwap);
    ttyDraw(consoleSize);
    ivec2 conSize = {0, 0};
    ttyGetSize(conSize);
    if (conSize[0] != consoleSize[0] || conSize[1] != consoleSize[1])
    {
        consoleSize[0] = conSize[0];
        consoleSize[1] = conSize[1];
        if (!renderResize(consoleSize, eglResize)) exit(1);
        ttyResize(consoleSize);
    }
}

void handleCtrlC()
{
    stop = true;
}

bool consoleInit(const char *flag)
{
    signal(SIGINT, handleCtrlC);

    ttyGetSize(consoleSize);

    if (consoleSize[0] <= 10 || consoleSize[1] <= 10)
    {
#ifdef ALLOW_INVALID_TERMINALS
        consoleSize[0] = 80;
        consoleSize[1] = 24;
#else
        fprintf(stderr, "Invalid console size: Must be at least 10x10.\n");
        return false;
#endif
    }

#ifndef ALLOW_INVALID_TERMINALS
    if (!ttySupportsTrueColor())
    {
        fprintf(stderr, "This terminal does not seem to support 24-bit color.\n");
        return false;
    }
#endif

    if (!ttyInit(consoleSize)) return false;
    if (!renderInit(consoleSize, flag, eglInit, true)) return false;

    return true;
}

void consoleLoop()
{
    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (!stop) consoleRenderFrame(); // This can be interrupted by Ctrl+C
}

void consoleCleanup()
{
    renderCleanup(eglCleanup);
    ttyCleanup();
    setvbuf(stdout, NULL, _IONBF, 0);
    fflush(stdout);
    printf(ANSI_SHOW_CURSOR); // Show cursor
    printf(ANSI_RESET_COLORS "\n"); // Reset colors
    printf(ANSI_CLEAR_SCREEN); // Clear screen
    printf("Goodbye\n");
}
