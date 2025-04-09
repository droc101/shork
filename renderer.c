//
// Created by droc101 on 4/8/25.
//

#include "renderer.h"
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <bits/signum-generic.h>
#include <cglm/cglm.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "io.h"
#include "model_loader.h"

EGLDisplay display;
EGLSurface surface;
EGLContext context;

ivec2 viewport = {0, 0};
unsigned int VBO = 0;
unsigned int EBO = 0;
unsigned int shaderProgram = 0;
GLuint texture = 0;
Model* shork = NULL;
mat4* modelViewProjectionMatrix = NULL;

bool eglInit(const ivec2 size)
{
    viewport[0] = size[0];
    viewport[1] = size[1] * 2;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY)
    {
        fprintf(stderr, "Failed to get EGL display\n");
        return false;
    }

    if (!eglInitialize(display, NULL, NULL))
    {
        fprintf(stderr, "Failed to initialize EGL\n");
        return false;
    }

    const EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_SAMPLES, 0,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0)
    {
        fprintf(stderr, "Failed to choose EGL config\n");
        return false;
    }
    const EGLint pbufferAttribs[] = {
        EGL_WIDTH, viewport[0],
        EGL_HEIGHT, viewport[1],
        EGL_NONE,
    };
    surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    if (surface == EGL_NO_SURFACE)
    {
        fprintf(stderr, "Failed to create EGL Pbuffer surface\n");
        return false;
    }
    if (!eglBindAPI(EGL_OPENGL_ES_API))
    {
        fprintf(stderr, "Failed to bind OpenGL ES API\n");
        return false;
    }
    const EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        fprintf(stderr, "Failed to create EGL context\n");
        return false;
    }
    if (!eglMakeCurrent(display, surface, surface, context))
    {
        fprintf(stderr, "Failed to make EGL context current\n");
        return false;
    }

    const char* renderer = (const char*)glGetString(GL_RENDERER);
    const char* version = (const char*)glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version: %s\n", version);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    shork = LoadModel("assets/shork.bin");
    if (shork == NULL)
    {
        fprintf(stderr, "Failed to load model\n");
        return false;
    }
    modelViewProjectionMatrix = eglGetWorldViewMatrix();
    if (modelViewProjectionMatrix == NULL)
    {
        fprintf(stderr, "Failed to get world view matrix\n");
        return false;
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * shork->vertexCount, shork->vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shork->indexCount * sizeof(uint), shork->indexData, GL_STATIC_DRAW);

    const GLuint vertexShader = eglCreateShader("assets/vertex.glsl", GL_VERTEX_SHADER);
    if (vertexShader == -1)
    {
        fprintf(stderr, "Failed to create vertex shader\n");
        return false;
    }
    const GLuint fragmentShader = eglCreateShader("assets/fragment.glsl", GL_FRAGMENT_SHADER);
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

    Image* haj_tex = readImage("assets/shork.png");
    if (haj_tex == NULL)
    {
        fprintf(stderr, "Failed to load texture\n");
        return false;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, haj_tex->width, haj_tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 haj_tex->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    free(haj_tex->data);
    free(haj_tex);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return true;
}

void eglGetModelWorldMatrix(mat4* transformMatrix, const double rotation)
{
    glm_mat4_identity(*transformMatrix);
    glm_rotate(*transformMatrix, (float)rotation, (vec3){0, 1, 0});
}

mat4* eglGetWorldViewMatrix()
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

GLuint eglCreateShader(const char* filename, const GLenum type)
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

void eglDrawFrame()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    const ulong ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    const double rot = (double)ms / 1000.0f;

    mat4 modelWorldMatrix = GLM_MAT4_IDENTITY_INIT;
    eglGetModelWorldMatrix(&modelWorldMatrix, rot);
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "PROJECTION"), 1, GL_FALSE, *modelViewProjectionMatrix[0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "WORLD"), 1, GL_FALSE, modelWorldMatrix[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "TEXTURE"), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, (GLsizei)shork->indexCount, GL_UNSIGNED_INT, 0);
    eglSwapBuffers(display, surface);
}

void eglCleanup()
{
    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    eglDestroySurface(display, surface);
    eglDestroyContext(display, context);
    eglTerminate(display);
}

void eglGetFramebuffer(void* buffer)
{
    glReadPixels(0, 0, viewport[0], viewport[1], GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}
