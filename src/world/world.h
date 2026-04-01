#pragma once
#define WORLD_WIDTH 200
#define WORLD_HEIGHT 30
#define WORLD_DEPTH 200

#include "utils/SimplexNoise.h"
#include "chunk/chunk.h"
#include "game/camera.h"
#include "blocks.h"

#include "player/physics.h"

class World {
private:
    static World* instance;

    PhysicsEntity physics;

    SimplexNoise noise;
    Block world[WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH] {Block::AIR};
    Chunk chunk;

    GameCamera camera;
    Shader shader;

    int viewPosLoc;
    int fogColLoc;
    int fogStartLoc;
    int fogEndLoc;

public:
    explicit World() {
        noise = SimplexNoise(1.0f, 1.0f, 100.0f, 1.0f);
        chunk = Chunk();
        camera = GameCamera();
        shader = LoadShader("shaders/fog.vert", "shaders/fog.frag");

        viewPosLoc = GetShaderLocation(shader, "viewPos");
        fogColLoc = GetShaderLocation(shader, "fogColor");
        fogStartLoc = GetShaderLocation(shader, "fogStart");
        fogEndLoc = GetShaderLocation(shader, "fogEnd");

        physics = PhysicsEntity(100.0f, 25.0f, 112.0f);

        instance = this;
    }

    void generateWorld();
    void update(float dt);
    void render();
    void dispose();

    Block* getWorld();

    static World& GetInstance() { return *instance; }

    static inline int GetIndex(int x, int y, int z) {
        return z * WORLD_WIDTH * WORLD_HEIGHT + y * WORLD_WIDTH + x;
    }
    static inline bool IsAir(Block* world, int x, int y, int z) {
        if (x < 0 || y < 0 || z < 0 ||
            x >= WORLD_WIDTH || y >= WORLD_HEIGHT || z >= WORLD_DEPTH)
            return true;

        return world[GetIndex(x,y,z)] == Block::AIR;
    }

private:
    int Clamp(int val, int min, int max) {
        if (val < min) return min;
        if (val > max) return max;
        return val;
    }
};