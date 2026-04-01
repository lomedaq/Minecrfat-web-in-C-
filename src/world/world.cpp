#include "world.h"
#include "raymath.h"
#include <iostream>

#include "generation/surface.h"
#include "generation/cave.h"

World* World::instance = nullptr;

void World::generateWorld() {
    std::cout << "========================================================================" << std::endl;
    SurfaceGeneration surface = SurfaceGeneration().Initialization();
    CaveGeneration cave = CaveGeneration().Initialization();

    surface.generation(world, noise);
    cave.generation(world, noise);

    std::cout << "========================================================================" << std::endl;
    chunk.UpdateChunkMesh(world);

    std::cout << "Vertex count: " << chunk.GetChunkMesh().meshes->vertexCount << std::endl;
}

void World::update(float dt) {
    camera.Update(physics.getPos(), dt);

    Vector3 moveDir = {0};
    if (IsKeyDown(KEY_W)) moveDir = Vector3Add(moveDir, camera.forward);
    if (IsKeyDown(KEY_S)) moveDir = Vector3Subtract(moveDir, camera.forward);
    if (IsKeyDown(KEY_D)) moveDir = Vector3Add(moveDir, camera.right);
    if (IsKeyDown(KEY_A)) moveDir = Vector3Subtract(moveDir, camera.right);
    moveDir.y = 0;

    physics.move(moveDir, dt);
    if (IsKeyPressed(KEY_SPACE)) physics.jump();
    physics.update(dt);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        Vector2 center = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        Ray ray = GetMouseRay(center, camera.GetCamera());

        float closestDist = 99999.0f;
        float maxReach = 6.0f;
        Vector3 hitBlockPos = { -1, -1, -1 };
        Vector3 hitNormal = { 0, 0, 0 };

        Vector3 pPos = physics.getPos();

        int startX = Clamp((int)pPos.x - (int)maxReach, 0, WORLD_WIDTH - 1);
        int endX = Clamp((int)pPos.x + (int)maxReach, 0, WORLD_WIDTH - 1);
        
        int startY = Clamp((int)pPos.y - (int)maxReach, 0, WORLD_HEIGHT - 1);
        int endY = Clamp((int)pPos.y + (int)maxReach, 0, WORLD_HEIGHT - 1);
        
        int startZ = Clamp((int)pPos.z - (int)maxReach, 0, WORLD_DEPTH - 1);
        int endZ = Clamp((int)pPos.z + (int)maxReach, 0, WORLD_DEPTH - 1);

        for (int x = startX; x <= endX; x++) {
            for (int y = startY; y <= endY; y++) {
                for (int z = startZ; z <= endZ; z++) {
                    if (world[GetIndex(x, y, z)] == Block::AIR) continue;

                    BoundingBox box = {
                        { (float)x, (float)y - 0.5f, (float)z }, 
                        { (float)x + 1.0f, (float)y + 0.5f, (float)z + 1.0f }
                    };

                    RayCollision col = GetRayCollisionBox(ray, box);
                    
                    if (col.hit && col.distance < closestDist && col.distance <= maxReach) {
                        closestDist = col.distance;
                        hitBlockPos = { (float)x, (float)y, (float)z };
                        hitNormal = col.normal;
                    }
                }
            }
        }

        if (hitBlockPos.x >= 0) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                world[GetIndex((int)hitBlockPos.x, (int)hitBlockPos.y, (int)hitBlockPos.z)] = Block::AIR;
            } 
            else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                int nx = (int)hitBlockPos.x + (int)std::round(hitNormal.x);
                int ny = (int)hitBlockPos.y + (int)std::round(hitNormal.y);
                int nz = (int)hitBlockPos.z + (int)std::round(hitNormal.z);

                if (nx >= 0 && nx < WORLD_WIDTH && ny >= 0 && ny < WORLD_HEIGHT && nz >= 0 && nz < WORLD_DEPTH) {
                    Vector3 playerRounded = { std::floor(pPos.x), std::floor(pPos.y), std::floor(pPos.z) };
                    if (!(nx == (int)playerRounded.x && nz == (int)playerRounded.z && 
                        (ny == (int)playerRounded.y || ny == (int)playerRounded.y + 1))) {
                        world[GetIndex(nx, ny, nz)] = Block::DIRT;
                    }
                }
            }

            chunk.UnloadModel();
            chunk.UpdateChunkMesh(world);
        }
    }
}

void World::render() {
    Camera3D cam = camera.GetCamera();

    Vector3 target = cam.up;
    Vector3 pos = physics.getPos();
    DrawText(
        TextFormat("Camera target: x=%.2f y=%.2f z=%.2f", target.x, target.y, target.z),
        100, 10, 20, RED
    );
    DrawText(
        TextFormat("Player pos: x=%.2f y=%.2f z=%.2f", pos.x, pos.y, pos.z),
        100, 40, 20, RED
    );
    BeginMode3D(camera.GetCamera());
        DrawGrid(10, 1.0f);
        chunk.SetChunkShader(shader);

        SetShaderValue(shader, viewPosLoc, &cam.position, SHADER_UNIFORM_VEC3);
        Vector3 fogCol = {0.8f, 0.8f, 0.8f}; 
        SetShaderValue(shader, fogColLoc, &fogCol, SHADER_UNIFORM_VEC3);

        float fogStart = 10.0f;
        float fogEnd = 50.0f;
        SetShaderValue(shader, fogStartLoc, &fogStart, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, fogEndLoc, &fogEnd, SHADER_UNIFORM_FLOAT);

        DrawModel(chunk.GetChunkMesh(), {0.5f,0,0.5f}, 1.0f, WHITE);

        // for (int x=0;x<WORLD_WIDTH;x++)
        //     for (int y=0;y<WORLD_HEIGHT;y++)
        //         for (int z=0;z<WORLD_DEPTH;z++) {
        //             if (world[GetIndex(x,y,z)]==Block::AIR) continue;

        //             Vector3 pos = {(float)x,(float)y,(float)z};
                    
        //             if (IsAir(world,x+1,y,z) || IsAir(world,x-1,y,z) ||
        //                 IsAir(world,x,y+1,z) || IsAir(world,x,y-1,z) ||
        //                 IsAir(world,x,y,z+1) || IsAir(world,x,y,z-1)) 
        //             {
        //                 DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, BLACK);
        //             }
        //         }
    EndMode3D();

    DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2, 2, RED);
}

Block* World::getWorld() {
    return world;
}

void World::dispose() {
    UnloadShader(shader);
    chunk.UnloadModel();
}