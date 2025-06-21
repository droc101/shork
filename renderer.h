//
// Created by droc101 on 4/8/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <stdbool.h>
#include <cglm/types.h>
#include <GLES3/gl3.h>

typedef bool (*RenderResizeFunc)(const ivec2 viewport);
typedef void (*RenderCleanupFunc)();
typedef void (*RenderSwapFunc)();
typedef bool (*RenderInitFunc)(ivec2 viewport);

bool renderInit(const ivec2 size, const char* overlayTexture, RenderInitFunc init, bool halfHeight);

void renderCleanup(RenderCleanupFunc cleanup);

void glRenderFrame(RenderSwapFunc swap);

/**
 * Copy the rendered frame to the buffer.
 * @param buffer The buffer to copy the frame to.
 */
void renderGetFramebuffer(void* buffer);

/**
 * Get the model world matrix.
 * @param transformMatrix The transform matrix to be filled.
 * @param rotation The rotation angle in degrees.
 */
void renderGetModelWorldMatrix(mat4* transformMatrix, double rotation);

/**
 * Get the world view matrix.
 * @return The world view matrix.
 */
mat4* renderGetWorldViewMatrix();

/**
 * Create a shader from a file.
 * @param filename The name of the file to load.
 * @param type The type of shader to create.
 * @return The shader ID.
 */
GLuint renderCreateShader(const char* filename, GLenum type);

bool renderResize(const ivec2 newSize, RenderResizeFunc resize);

/**
 * Get the current viewport size in pixels.
 */
const ivec2 *getViewport();

#endif //RENDERER_H
