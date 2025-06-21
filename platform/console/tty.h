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
void ttyGetSize(ivec2 size);

/**
 * Initialize the console memory.
 * @param size The size of the console in characters.
 * @return whether the initialization was successful.
 */
bool ttyInit(const ivec2 size);

/**
 * Draw the framebuffer to the console.
 * @param viewport The size of the console in characters.
 */
void ttyDraw(const ivec2 viewport);

/**
 * Check if the console supports true color. ($COLORTERM = "truecolor" or "24bit")
 * @return Whether the console supports true color.
 */
bool ttySupportsTrueColor();

/**
 * Cleanup the console memory.
 */
void ttyCleanup();

/**
 * Resize the console framebuffer.
 * @param newSize The new size of the console in characters.
 * @note This entirely recreates the framebuffer and output buffer.
 */
void ttyResize(const ivec2 newSize);

#endif //CONSOLE_H
