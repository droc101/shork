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
 * @return whether the initialization was successful.
 */
bool eglInit(const ivec2 size);

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

#endif //RENDERER_H
