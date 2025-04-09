//
// Created by droc101 on 4/8/25.
//

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

typedef struct Model Model;
typedef struct ModelHeader ModelHeader;

struct ModelHeader
{
    /// The "magic" for the header, should be "MSH"
    char sig[4];
    /// The number of indices in the model
    unsigned int indexCount;
    /// The number of vertices in the model
    unsigned int vertexCount;
    /// The "magic" for the data, should be "DAT"
    char dataSig[4];
} __attribute__((packed));

struct Model
{
    ModelHeader header;

    /// The number of vertices in the model
    unsigned int vertexCount;
    /// The number of indices in the model
    unsigned int indexCount;
    /// Packed vertex data, (X Y Z) (U V) (NX NY NZ)
    float* vertexData;
    /// Index data
    unsigned int* indexData;
};

/**
 * Load a model from a file.
 * @param asset The path to the model file.
 * @return The loaded model, or NULL if the model could not be loaded.
 */
Model* LoadModel(const char* asset);

#endif //MODEL_LOADER_H
