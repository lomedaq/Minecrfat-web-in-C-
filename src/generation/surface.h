#pragma once
#include "utils/SimplexNoise.h"
#include "blocks.h"

class SurfaceGeneration {
private:
    float scale = 0.05f;

    Block air;
    Block grass;
    Block dirt;
    Block stone;

public:
    SurfaceGeneration Initialization();
    void generation(Block* world, SimplexNoise noise);
};