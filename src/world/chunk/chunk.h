#pragma once
#include "raylib.h"

#include "blocks.h"

class Chunk {
private:
    Mesh chunkMesh;
    Model chunkModel;

    Mesh GenerateChunkMesh(Block* world);
public:

    void UpdateChunkMesh(Block* world);
    void SetChunkShader(Shader shader);
    Model GetChunkMesh();
    void UnloadModel();
};