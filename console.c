//
// Created by droc101 on 4/8/25.
//

#include "console.h"
#include <string.h>
#include <sys/ioctl.h>
#define STB_SPRINTF_IMPLEMENTATION
#include "stb/stb_sprintf.h"
#include "renderer.h"

void* framebuffer = NULL;
char* outputBuffer = NULL;

void getConsoleSize(ivec2 size)
{
    struct winsize w;
    if (ioctl(0, TIOCGWINSZ, &w) == -1)
    {
        size[0] = -1;
        size[1] = -1;
        return;
    }
    size[0] = w.ws_col;
    size[1] = w.ws_row;
}

void consoleInit(const ivec2 size)
{
    framebuffer = malloc(size[0] * size[1] * 8);
    if (framebuffer == NULL)
    {
        fprintf(stderr, "Failed to allocate framebuffer\n");
        return;
    }
    size_t outputBufferSize = sizeof("\033[48;2;100;100;100m\033[38;2;100;100;100m") * size[0] * size[1];
    outputBufferSize += sizeof("\033[0m\n") * size[1];
    outputBufferSize += 256;
    outputBuffer = malloc(outputBufferSize);
    if (outputBuffer == NULL)
    {
        fprintf(stderr, "Failed to allocate output buffer\n");
        free(framebuffer);
        return;
    }
    setvbuf(stdout, outputBuffer, _IOFBF, 1 << 30);
    printf("\033[?25l"); // Hide cursor
    fflush(stdout);
}

void consoleDraw(const ivec2 viewport)
{
    eglGetFramebuffer(framebuffer);
    char* buf = outputBuffer;
    stbsp_sprintf(buf, "\033[H"); // Move cursor to home position
    buf += strlen(buf);
    for (int hy = viewport[1] - 1; hy >= 0; hy--)
    {
        for (int x = 0; x < viewport[0]; x++)
        {
            const int y = hy * 2;
            const unsigned char* pixel = (unsigned char*)framebuffer + ((y + 1) * viewport[0] + x) * 4;
            const unsigned char* pixelBelow = (unsigned char*)framebuffer + (y * viewport[0] + x) * 4;
            stbsp_sprintf(buf, "\033[48;2;%d;%d;%dm\033[38;2;%d;%d;%dm▄", pixel[0], pixel[1], pixel[2], pixelBelow[0],
                          pixelBelow[1], pixelBelow[2]);
            buf += strlen(buf);
        }
        stbsp_sprintf(buf, "\033[0m\n"); // Reset colors and move to next line
        buf += strlen(buf);
    }
    buf -= 1;
    *buf = '\0';
    fwrite(outputBuffer, strlen(outputBuffer), 1, stdout);
    fflush(stdout);
}
