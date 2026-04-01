#pragma once

#include "raylib.h"

class PhysicsEntity {
private:
    Vector3 pos;
    Vector3 vel;
    float eps = 0.01f;

    float gravity;
    float moveSpeed;
    float halfSize;
    float playerHeight;

    bool onGround;

    bool isSolid(int x, int y, int z);
    bool checkCollision(float x, float y, float z);

public:
    PhysicsEntity(float x = 0, float y = 0, float z = 0);

    void move(Vector3 dir, float dt);
    void jump();
    void update(float dt);

    Vector3 getPos();
};