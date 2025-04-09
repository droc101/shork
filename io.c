//
// Created by droc101 on 4/8/25.
//

#include "io.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


char* readFile(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    const size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = calloc(size + 1, 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for file: %s\n", filename);
        fclose(f);
        return NULL;
    }
    fread(buffer, 1, size, f);
    fclose(f);
    return buffer;
}

void* readFileRaw(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    const size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    void* buffer = malloc(size);
    if (buffer == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for file: %s\n", filename);
        fclose(f);
        return NULL;
    }
    fread(buffer, 1, size, f);
    fclose(f);
    return buffer;
}

Image* readImage(const char* filename)
{
    Image* image = malloc(sizeof(Image));
    image->data = stbi_load(filename, &image->width, &image->height, &image->channels, 0);
    if (image->data == NULL)
    {
        fprintf(stderr, "Failed to load image: %s\n", filename);
        return NULL;
    }
    if (image->channels != 4)
    {
        fprintf(stderr, "Unsupported image format: %s\n", filename);
        stbi_image_free(image->data);
        free(image);
        return NULL;
    }
    return image;
}
