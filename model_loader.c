//
// Created by droc101 on 4/8/25.
//

#include "model_loader.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"

Model* LoadModel(const char* asset)
{
    void* file = readFileRaw(asset);
    Model* model = malloc(sizeof(Model));
    memcpy(&model->header, file, sizeof(ModelHeader));

    if (strncmp(model->header.sig, "MSH", 3) != 0)
    {
        printf("Tried to load a model, but its first magic was incorrect (got %s)!", model->header.sig);
        free(model);
        free(file);
        return NULL;
    }

    if (strncmp(model->header.dataSig, "DAT", 3) != 0)
    {
        printf("Tried to load a model, but its second magic was incorrect (got %s)!", model->header.dataSig);
        free(model);
        free(file);
        return NULL;
    }

    const size_t vertsSizeBytes = model->header.vertexCount * (sizeof(float) * 8);
    const size_t indexSizeBytes = model->header.indexCount * sizeof(uint);

    model->vertexCount = model->header.vertexCount;
    model->indexCount = model->header.indexCount;

    model->vertexData = malloc(vertsSizeBytes);
    model->indexData = malloc(indexSizeBytes);

    // Copy the index data, then the vertex data
    memcpy(model->indexData, file + sizeof(ModelHeader), indexSizeBytes);
    memcpy(model->vertexData, file + sizeof(ModelHeader) + indexSizeBytes, vertsSizeBytes);

    free(file);

    return model;
}
