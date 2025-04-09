#include <signal.h>

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
    printf("\033[?25h"); // Show cursor
    printf("\033[0m\n"); // Reset colors
    printf("\033[H\033[J"); // Clear screen
    printf("Goodbye\n");
    fflush(stdout);
    eglCleanup();
    exit(0);
}

int main(void)
{
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
    consoleInit(consoleSize);
    eglInit(consoleSize);

    while (1) renderFrame();
}
