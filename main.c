#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "ansi.h"
#include "console.h"
#include "renderer.h"

//#define ALLOW_INVALID_TERMINALS

ivec2 consoleSize = {0, 0};

void renderFrame()
{
    eglDrawFrame();
    consoleDraw(consoleSize);
}

void handleCtrlC()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf(ANSI_SHOW_CURSOR); // Show cursor
    printf(ANSI_RESET_COLORS "\n"); // Reset colors
    printf(ANSI_CLEAR_SCREEN); // Clear screen
    printf("Goodbye\n");
    fflush(stdout);
    eglCleanup();
    exit(0);
}

int main(int argc, char *argv[])
{
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

    if (!consoleSupportsTrueColor())
    {
        fprintf(stderr, "This terminal does not seem to support 24-bit color.\n");
        return 1;
    }

    if (!consoleInit(consoleSize)) exit(1);
    if (!eglInit(consoleSize)) exit(1);

    while (1) renderFrame(); // This can be interrupted by Ctrl+C
}
