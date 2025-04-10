#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "ansi.h"
#include "console.h"
#include "renderer.h"
#include "args.h"

// Uncomment to allow terminals that do not meet requirements (at least 10x10, 24bit color) to run.
//#define ALLOW_INVALID_TERMINALS

ivec2 consoleSize = {0, 0};
bool stop = false;
ulong frames = 0;

void renderFrame()
{
    eglDrawFrame();
    consoleDraw(consoleSize);
    frames++;
    ivec2 conSize = {0, 0};
    getConsoleSize(conSize);
    if (conSize[0] != consoleSize[0] || conSize[1] != consoleSize[1])
    {
        consoleSize[0] = conSize[0];
        consoleSize[1] = conSize[1];
        if (!eglResize(consoleSize)) exit(1);
        consoleResize(consoleSize);
    }
}

void handleCtrlC()
{
    stop = true;
}

int main(const int argc, char *argv[])
{
    if (has_arg(argc, argv, "-h") || has_arg(argc, argv, "--help"))
    {
        printf("Usage: %s [--flag=<value>]\n", argv[0]);
        printf("Options:\n");
        printf("  --flag=<value>  Set the flag value (default: rainbow)\n");
        printf("Flags should be placed in \"assets/flags\" named <value>.png\n");
        return 0;
    }

    char *path = argv[0];
    char *lastSlash = strrchr(path, '/');
    if (lastSlash != NULL)
    {
        *lastSlash = '\0';
    }
    else
    {
        path = ".";
    }
    if (chdir(path) != 0)
    {
        fprintf(stderr, "Error changing directory to %s\n", path);
        return 1;
    }

    signal(SIGINT, handleCtrlC);

    getConsoleSize(consoleSize);

    if (consoleSize[0] <= 10 || consoleSize[1] <= 10)
    {
#ifdef ALLOW_INVALID_TERMINALS
        consoleSize[0] = 80;
        consoleSize[1] = 24;
#else
        fprintf(stderr, "Invalid console size: Must be at least 10x10.\n");
        return 1;
#endif
    }

#ifndef ALLOW_INVALID_TERMINALS
    if (!consoleSupportsTrueColor())
    {
        fprintf(stderr, "This terminal does not seem to support 24-bit color.\n");
        return 1;
    }
#endif


    const char *flag = get_arg(argc, argv, "--flag", "rainbow");

    if (!consoleInit(consoleSize)) exit(1);
    if (!eglInit(consoleSize, flag)) exit(1);

    while (!stop) renderFrame(); // This can be interrupted by Ctrl+C

    eglCleanup();
    consoleCleanup();
    setvbuf(stdout, NULL, _IONBF, 0);
    fflush(stdout);
    printf(ANSI_SHOW_CURSOR); // Show cursor
    printf(ANSI_RESET_COLORS "\n"); // Reset colors
    printf(ANSI_CLEAR_SCREEN); // Clear screen
    printf("Goodbye\n");
    fflush(stdout);
    return 0;
}
