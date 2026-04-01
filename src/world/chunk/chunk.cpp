#include "chunk.h"

#include <vector>
#include <cstring>
#include "raymath.h"

#include "world/world.h"

Mesh Chunk::GenerateChunkMesh(Block* world) {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;
    std::vector<unsigned char> colors;

    for (int x = 0; x < WORLD_WIDTH; x++) {
        for (int y = 0; y < WORLD_HEIGHT; y++) {
            for (int z = 0; z < WORLD_DEPTH; z++) {
                Block block = world[World::GetIndex(x, y, z)];
                if (block == Block::AIR) continue;

                Vector3 pos = {(float)x, (float)y, (float)z};
                Color col;
                switch (block) {
                    case Block::GRASS: col = GREEN; break;
                    case Block::DIRT:  col = BEIGE; break;
                    case Block::STONE: col = GRAY; break;
                    default: col = WHITE;
                }

                static const Vector3 faceVerts[6][6] = {
                    // +X
                    {{0.5f,-0.5f,-0.5f},{0.5f,0.5f,-0.5f},{0.5f,0.5f,0.5f},
                    {0.5f,-0.5f,-0.5f},{0.5f,0.5f,0.5f},{0.5f,-0.5f,0.5f}},
                    // -X
                    {{-0.5f,-0.5f,0.5f},{-0.5f,0.5f,0.5f},{-0.5f,0.5f,-0.5f},
                    {-0.5f,-0.5f,0.5f},{-0.5f,0.5f,-0.5f},{-0.5f,-0.5f,-0.5f}},
                    // +Y
                    {{-0.5f,0.5f,-0.5f},{-0.5f,0.5f,0.5f},{0.5f,0.5f,0.5f},
                    {-0.5f,0.5f,-0.5f},{0.5f,0.5f,0.5f},{0.5f,0.5f,-0.5f}},
                    // -Y
                    {{-0.5f,-0.5f,-0.5f},{0.5f,-0.5f,-0.5f},{0.5f,-0.5f,0.5f},
                    {-0.5f,-0.5f,-0.5f},{0.5f,-0.5f,0.5f},{-0.5f,-0.5f,0.5f}},
                    // +Z
                    {{-0.5f,-0.5f,0.5f},{0.5f,-0.5f,0.5f},{0.5f,0.5f,0.5f},
                    {-0.5f,-0.5f,0.5f},{0.5f,0.5f,0.5f},{-0.5f,0.5f,0.5f}},
                    // -Z
                    {{0.5f,-0.5f,-0.5f},{-0.5f,-0.5f,-0.5f},{-0.5f,0.5f,-0.5f},
                    {0.5f,-0.5f,-0.5f},{-0.5f,0.5f,-0.5f},{0.5f,0.5f,-0.5f}},
                };

                static const Vector3 normalsDir[6] = {
                    {1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
                };

                for (int face = 0; face < 6; face++) {
                    int nx = x + ((face==0)?1:(face==1)?-1:0);
                    int ny = y + ((face==2)?1:(face==3)?-1:0);
                    int nz = z + ((face==4)?1:(face==5)?-1:0);
                    if (!World::IsAir(world, nx, ny, nz)) continue;

                    for (int i = 0; i < 6; i++) {
                        Vector3 v = Vector3Add(pos, faceVerts[face][i]);
                        vertices.push_back(v.x); vertices.push_back(v.y); vertices.push_back(v.z);

                        normals.push_back(normalsDir[face].x);
                        normals.push_back(normalsDir[face].y);
                        normals.push_back(normalsDir[face].z);

                        texcoords.push_back((i==1||i==2||i==4)?1.0f:0.0f);
                        texcoords.push_back((i>=2 && i<=4)?1.0f:0.0f);

                        colors.push_back(col.r); colors.push_back(col.g);
                        colors.push_back(col.b); colors.push_back(col.a);
                    }
                }
            }
        }
    }

    Mesh mesh = {0};
    mesh.vertexCount = vertices.size()/3;
    mesh.triangleCount = mesh.vertexCount/3;

    mesh.vertices  = (float*)MemAlloc(vertices.size()*sizeof(float));
    mesh.normals   = (float*)MemAlloc(normals.size()*sizeof(float));
    mesh.texcoords = (float*)MemAlloc(texcoords.size()*sizeof(float));
    mesh.colors    = (unsigned char*)MemAlloc(colors.size()*sizeof(unsigned char));

    memcpy(mesh.vertices, vertices.data(), vertices.size()*sizeof(float));
    memcpy(mesh.normals,  normals.data(), normals.size()*sizeof(float));
    memcpy(mesh.texcoords,texcoords.data(), texcoords.size()*sizeof(float));
    memcpy(mesh.colors,   colors.data(), colors.size()*sizeof(unsigned char));

    UploadMesh(&mesh, false);
    return mesh;
}

void Chunk::UpdateChunkMesh(Block* world) {
    chunkMesh = GenerateChunkMesh(world);
    chunkModel = LoadModelFromMesh(chunkMesh);
}

void Chunk::SetChunkShader(Shader shader) {
    chunkModel.materials[0].shader = shader;
}

Model Chunk::GetChunkMesh() {
    return chunkModel;
}

void Chunk::UnloadModel() {
    ::UnloadModel(chunkModel);
}