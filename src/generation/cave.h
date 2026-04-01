#pragma once
#include "utils/SimplexNoise.h"
#include "blocks.h"

class CaveGeneration {
private:
    float scale = 0.05f;
    Block air;

public:
    CaveGeneration Initialization();
    void generation(Block* world, SimplexNoise noise);
};