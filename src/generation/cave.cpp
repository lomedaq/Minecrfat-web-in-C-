#include "cave.h"

#include "world/world.h"

CaveGeneration CaveGeneration::Initialization() {
    air = Block::AIR;

    return *this;
}

void CaveGeneration::generation(Block* world, SimplexNoise noise) {
    for (int x=0;x<WORLD_WIDTH;x++)
        for (int y=0;y<WORLD_HEIGHT;y++)
            for (int z=0;z<WORLD_DEPTH;z++) 
                if (noise.noise(x*scale,y*scale,z*scale) < 0.5f) world[World::GetIndex(x,y,z)] = air;
}