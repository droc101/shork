//
// Created by droc101 on 4/8/25.
//

#ifndef CONSOLE_H
#define CONSOLE_H
#include <cglm/cglm.h>

/**
 * Get the size of the console in characters.
 * @param size The ivec2 to store the size in.
 */
void getConsoleSize(ivec2 size);

/**
 * Initialize the console memory.
 * @param size The size of the console in characters.
 */
void consoleInit(const ivec2 size);

/**
 * Draw the framebuffer to the console.
 * @param viewport The size of the console in characters.
 */
void consoleDraw(const ivec2 viewport);

#endif //CONSOLE_H
