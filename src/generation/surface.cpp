#include "surface.h"

#include "world/world.h"

SurfaceGeneration SurfaceGeneration::Initialization() {
    air = Block::AIR;
    grass = Block::GRASS;
    dirt = Block::DIRT;
    stone = Block::STONE;

    return *this;
}

void SurfaceGeneration::generation(Block* world, SimplexNoise noise) {
    int heightMap[WORLD_WIDTH*WORLD_DEPTH]{0};

    for (int x=0;x<WORLD_WIDTH;x++)
        for (int z=0;z<WORLD_DEPTH;z++)
            heightMap[x*WORLD_DEPTH+z] = WORLD_HEIGHT-10 + (int)(noise.noise(x*scale,z*scale)*5);

    for (int x=0;x<WORLD_WIDTH;x++)
        for (int y=0;y<WORLD_HEIGHT;y++)
            for (int z=0;z<WORLD_DEPTH;z++) {
                world[World::GetIndex(x,y,z)] = air;
                if (y>heightMap[x*WORLD_DEPTH+z]) continue;

                if (y==heightMap[x*WORLD_DEPTH+z]) world[World::GetIndex(x,y,z)] = grass;
                else if (y>heightMap[x*WORLD_DEPTH+z]-10) world[World::GetIndex(x,y,z)] = dirt;
                else world[World::GetIndex(x,y,z)] = stone;
            }
}