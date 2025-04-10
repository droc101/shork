//
// Created by droc101 on 4/8/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <stdbool.h>
#include <cglm/types.h>
#include <GLES3/gl3.h>

/**
 * Initializes EGL and creates and offscreen framebuffer with 2x the height of the size.
 * @param size The size of the framebuffer.
 * @param overlayTexture
 * @return whether the initialization was successful.
 */
bool eglInit(const ivec2 size, const char* overlayTexture);

/**
 * Cleans up EGL resources.
 */
void eglCleanup();

/**
 * Draw a frame
 */
void eglDrawFrame();

/**
 * Copy the rendered frame to the buffer.
 * @param buffer The buffer to copy the frame to.
 */
void eglGetFramebuffer(void* buffer);

/**
 * Get the model world matrix.
 * @param transformMatrix The transform matrix to be filled.
 * @param rotation The rotation angle in degrees.
 */
void eglGetModelWorldMatrix(mat4* transformMatrix, double rotation);

/**
 * Get the world view matrix.
 * @return The world view matrix.
 */
mat4* eglGetWorldViewMatrix();

/**
 * Create a shader from a file.
 * @param filename The name of the file to load.
 * @param type The type of shader to create.
 * @return The shader ID.
 */
GLuint eglCreateShader(const char* filename, GLenum type);

/**
 * Resize the framebuffer.
 * @param newSize The new size of the framebuffer in characters. Will be doubled in height.
 * @return Whether the resize was successful.
 */
bool eglResize(const ivec2 newSize);

/**
 * Get the current viewport size in pixels.
 */
const ivec2 *getViewport();

#endif //RENDERER_H
