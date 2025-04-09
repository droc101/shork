//
// Created by droc101 on 4/8/25.
//

#ifndef IO_H
#define IO_H

typedef struct Image
{
    // The width of the image in pixels
    int width;
    // The height of the image in pixels
    int height;
    // The number of channels in the image
    int channels;
    // The image data
    unsigned char *data;
} Image;

/**
 * Read a text file into a string.
 * @param filename The name of the file to read.
 * @return The contents of the file as a string, or NULL if the file could not be read.
 */
char *readFile(const char *filename);

/**
 * Read a binary file into a buffer.
 * @param filename The name of the file to read.
 * @return The contents of the file as a buffer, or NULL if the file could not be read.
 */
void *readFileRaw(const char *filename);

/**
 * Read an image from a file.
 * @param filename The name of the file to read.
 * @return The image data, or NULL if the file could not be read.
 */
Image *readImage(const char *filename);

#endif //IO_H
