//
// Created by droc101 on 4/8/25.
//

#include "console.h"
#include <string.h>
#include <sys/ioctl.h>
#define STB_SPRINTF_IMPLEMENTATION
#include "ansi.h"
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

bool consoleInit(const ivec2 size)
{
    framebuffer = malloc(size[0] * size[1] * 8);
    if (framebuffer == NULL)
    {
        fprintf(stderr, "Failed to allocate framebuffer\n");
        return false;
    }
    size_t outputBufferSize = sizeof(ANSI_FG_COLOR(255,255,255) ANSI_BG_COLOR(255,255,255)) * size[0] * size[1];
    outputBufferSize += sizeof(ANSI_RESET_COLORS "\n") * size[1];
    outputBufferSize += 256;
    outputBuffer = malloc(outputBufferSize);
    if (outputBuffer == NULL)
    {
        fprintf(stderr, "Failed to allocate output buffer\n");
        free(framebuffer);
        return false;
    }
    setvbuf(stdout, outputBuffer, _IOFBF, 1 << 30);
    printf(ANSI_HIDE_CURSOR); // Hide cursor
    fflush(stdout);
    return true;
}

void consoleDraw(const ivec2 viewport)
{
    eglGetFramebuffer(framebuffer);
    char* buf = outputBuffer;
    stbsp_sprintf(buf, ANSI_MOVE_CURSOR_TOP_LEFT); // Move cursor to home position
    buf += strlen(buf);
    for (int hy = viewport[1] - 1; hy >= 0; hy--)
    {
        for (int x = 0; x < viewport[0]; x++)
        {
            const int y = hy * 2;
            const unsigned char* pixel = (unsigned char*)framebuffer + ((y + 1) * viewport[0] + x) * 4;
            const unsigned char* pixelBelow = (unsigned char*)framebuffer + (y * viewport[0] + x) * 4;
            stbsp_sprintf(buf, ANSI_BG_COLOR_FMT ANSI_FG_COLOR_FMT "▄", pixel[0], pixel[1], pixel[2], pixelBelow[0],
                          pixelBelow[1], pixelBelow[2]);
            buf += strlen(buf);
        }
        stbsp_sprintf(buf, ANSI_RESET_COLORS "\n"); // Reset colors and move to next line
        buf += strlen(buf);
    }
    buf -= 1;
    *buf = '\0';
    fwrite(outputBuffer, strlen(outputBuffer), 1, stdout);
    fflush(stdout);
}

bool consoleSupportsTrueColor()
{
    const char* colorterm = getenv("COLORTERM");
    if (colorterm != NULL)
    {
        if (strstr(colorterm, "truecolor") || strstr(colorterm, "24bit"))
        {
            return true;
        }
    }
    return false;
}
