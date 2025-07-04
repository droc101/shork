//
// Created by droc101 on 4/8/25.
//

#include "renderer.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <bits/signum-generic.h>
#include <cglm/cglm.h>
#include <GLES3/gl3.h>
#include "io.h"
#include "model_loader.h"

ivec2 viewport = {0, 0};
unsigned int VBO = 0;
unsigned int EBO = 0;
unsigned int shaderProgram = 0;
GLuint albedo_texture = 0;
GLuint overlay_texture = 0;
Model* shork = NULL;
mat4* modelViewProjectionMatrix = NULL;
bool useHalfHeight = false;

bool loadTexture(const char* path, GLuint *texture, const bool filter)
{
    Image* img = readImage(path);
    if (img == NULL)
    {
        fprintf(stderr, "Failed to load texture\n");
        return false;
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 img->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    free(img->data);
    free(img);

    return true;
}

bool renderInit(const ivec2 size, const char* overlayTexture, const RenderInitFunc init, const bool halfHeight)
{
    useHalfHeight = halfHeight;

    viewport[0] = size[0];
    viewport[1] = size[1];
    if (useHalfHeight)
    {
        viewport[1] *= 2;
    }

    if (init && !init(viewport))
    {
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    shork = LoadModel("assets/shork.bin");
    if (shork == NULL)
    {
        fprintf(stderr, "Failed to load model\n");
        return false;
    }
    modelViewProjectionMatrix = renderGetWorldViewMatrix();
    if (modelViewProjectionMatrix == NULL)
    {
        fprintf(stderr, "Failed to get world view matrix\n");
        return false;
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(8 * sizeof(float) * shork->vertexCount), shork->vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(shork->indexCount * sizeof(uint)), shork->indexData, GL_STATIC_DRAW);

    const GLuint vertexShader = renderCreateShader("assets/vertex.glsl", GL_VERTEX_SHADER);
    if (vertexShader == -1)
    {
        fprintf(stderr, "Failed to create vertex shader\n");
        return false;
    }
    const GLuint fragmentShader = renderCreateShader("assets/fragment.glsl", GL_FRAGMENT_SHADER);
    if (fragmentShader == -1)
    {
        fprintf(stderr, "Failed to create fragment shader\n");
        return false;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader program link failed: %s\n", infoLog);
        return false;
    }
    glUseProgram(shaderProgram);

    const size_t flagPathSize = strlen("assets/flags/") + strlen(overlayTexture) + strlen(".png") + 1;
    char* flagPath = malloc(flagPathSize);
    snprintf(flagPath, flagPathSize, "assets/flags/%s.png", overlayTexture);

    if (!loadTexture("assets/shork.png", &albedo_texture, true)) return false;
    if (!loadTexture(flagPath, &overlay_texture, true))
    {
        fprintf(stderr, "Failed to load flag texture\n");
        free(flagPath);
        return false;
    }
    free(flagPath);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, overlay_texture);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glBindVertexArray(VBO);
    const GLint posAttrib = glGetAttribLocation(shaderProgram, "VERTEX");
    const GLint uvAttrib = glGetAttribLocation(shaderProgram, "VERTEX_UV");
    const GLint normAttrib = glGetAttribLocation(shaderProgram, "VERTEX_NORMAL");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(uvAttrib);
    glEnableVertexAttribArray(normAttrib);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "PROJECTION"), 1, GL_FALSE, *modelViewProjectionMatrix[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "ALBEDO_TEXTURE"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "OVERLAY_TEXTURE"), 1);
    const vec2 screenSize = {(float)viewport[0], (float)viewport[1]};
    glUniform2fv(glGetUniformLocation(shaderProgram, "SCREEN_SIZE"), 1, screenSize);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    const char* renderer = (const char*)glGetString(GL_RENDERER);
    const char* version = (const char*)glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version: %s\n", version);

    return true;
}

void renderGetModelWorldMatrix(mat4* transformMatrix, const double rotation)
{
    glm_mat4_identity(*transformMatrix);
    glm_rotate(*transformMatrix, (float)rotation, (vec3){0, 1, 0});
}

mat4* renderGetWorldViewMatrix()
{
    vec3 cameraPosition = {0, 0.5f, 3};
    const float aspectRatio = (float)viewport[0] / (float)viewport[1];

    mat4 identityMatrix = GLM_MAT4_IDENTITY_INIT;
    mat4 perspectiveMatrix = GLM_MAT4_ZERO_INIT;
    glm_perspective(glm_rad(90.0f), aspectRatio, 0.01f, 1000.0f, perspectiveMatrix);

    // Add camera position
    mat4 viewMatrix = GLM_MAT4_ZERO_INIT;
    glm_lookat(cameraPosition, (vec3){0, 0, 0}, (vec3){0, 1, 0}, viewMatrix);

    mat4 modelViewMatrix = GLM_MAT4_ZERO_INIT;
    glm_mat4_mul(viewMatrix, identityMatrix, modelViewMatrix);

    mat4* modelViewProjectionMatrix = malloc(sizeof(mat4));
    if (modelViewProjectionMatrix == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for modelViewProjectionMatrix\n");
        return NULL;
    }
    glm_mat4_mul(perspectiveMatrix, modelViewMatrix, *modelViewProjectionMatrix);

    return modelViewProjectionMatrix;
}

GLuint renderCreateShader(const char* filename, const GLenum type)
{
    const GLchar* shaderSource = readFile(filename);
    if (shaderSource == NULL)
    {
        fprintf(stderr, "Failed to read shader file: %s\n", filename);
        return -1;
    }
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader compile failed: %s\n", infoLog);
        glDeleteShader(shader);
        raise(SIGABRT);
        return -1;
    }
    free((void*)shaderSource);
    return shader;
}

void glRenderFrame(const RenderSwapFunc swap)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    const ulong ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    const double rot = (double)ms / 1000.0f;

    mat4 modelWorldMatrix = GLM_MAT4_IDENTITY_INIT;
    renderGetModelWorldMatrix(&modelWorldMatrix, rot);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "WORLD"), 1, GL_FALSE, modelWorldMatrix[0]);
    glDrawElements(GL_TRIANGLES, (GLsizei)shork->indexCount, GL_UNSIGNED_INT, 0);

    if (swap) swap();
}

void renderCleanup(const RenderCleanupFunc cleanup)
{
    glDeleteTextures(1, &albedo_texture);
    glDeleteTextures(1, &overlay_texture);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    if (cleanup) cleanup();
}

void renderGetFramebuffer(void* buffer)
{
    glReadPixels(0, 0, viewport[0], viewport[1], GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

bool renderResize(const ivec2 newSize, const RenderResizeFunc resize)
{
    viewport[0] = newSize[0];
    viewport[1] = newSize[1];
    if (useHalfHeight)
    {
        viewport[1] *= 2;
    }
    glViewport(0, 0, viewport[0], viewport[1]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glDeleteRenderbuffers(1, &VBO);
    glDeleteFramebuffers(1, &EBO);

    if (resize && !resize(viewport))
    {
        return false;
    }

    free(modelViewProjectionMatrix);
    modelViewProjectionMatrix = renderGetWorldViewMatrix();

    const vec2 screenSize = {(float)viewport[0], (float)viewport[1]};
    glUniform2fv(glGetUniformLocation(shaderProgram, "SCREEN_SIZE"), 1, screenSize);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "PROJECTION"), 1, GL_FALSE, *modelViewProjectionMatrix[0]);

    return true;
}

const ivec2 *getViewport()
{
    return &viewport;
}
