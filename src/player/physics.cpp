#include "physics.h"
#include <cmath>

#include "world/world.h"

PhysicsEntity::PhysicsEntity(float x, float y, float z) {
    pos = {x, y, z};
    vel = {0, 0, 0};

    gravity = 20.0f;
    moveSpeed = 6.0f;

    halfSize = 0.25f;
    playerHeight = 1.8f;

    onGround = false;
}

Vector3 PhysicsEntity::getPos() {
    return pos;
}

void PhysicsEntity::move(Vector3 dir, float dt) {
    dir.y = 0;

    if (Vector3Length(dir) > 0.0f)
        dir = Vector3Normalize(dir);

    vel.x = dir.x * moveSpeed;
    vel.z = dir.z * moveSpeed;
}

void PhysicsEntity::jump() {
    if (onGround) {
        vel.y = 8.0f;
        onGround = false;
    }
}

bool PhysicsEntity::isSolid(int x, int y, int z) {
    return !World::IsAir(World::GetInstance().getWorld(), x, y, z);
}

bool PhysicsEntity::checkCollision(float x, float y, float z) {
    int minX = (int)std::floor(x - halfSize + eps);
    int maxX = (int)std::floor(x + halfSize - eps);
    int minZ = (int)std::floor(z - halfSize + eps);
    int maxZ = (int)std::floor(z + halfSize - eps);

    int minY = (int)std::floor(y + eps);
    int maxY = (int)std::floor(y + playerHeight - eps);

    for (int bx = minX; bx <= maxX; bx++) {
        for (int bz = minZ; bz <= maxZ; bz++) {
            for (int by = minY; by <= maxY; by++) {
                if (isSolid(bx, by, bz)) return true;
            }
        }
    }
    return false;
}

void PhysicsEntity::update(float dt) {
    vel.y -= gravity * dt;

    int steps = 4;
    float stepDt = dt / steps;

    for (int i = 0; i < 4; i++) {
        pos.y += vel.y * stepDt;
        if (checkCollision(pos.x, pos.y, pos.z)) {
            if (vel.y < 0) {
                pos.y = std::floor(pos.y + 0.001f) + 1.0f;
                onGround = true;
            } else {
                pos.y = std::floor(pos.y + playerHeight - 0.001f) - playerHeight;
            }
            vel.y = 0;
        } else {
            onGround = checkCollision(pos.x, pos.y - 0.01f, pos.z);
        }

        pos.x += vel.x * stepDt;
        if (checkCollision(pos.x, pos.y, pos.z)) {
            if (vel.x > 0) pos.x = std::floor(pos.x + halfSize + eps) - halfSize - 0.001f;
            else pos.x = std::floor(pos.x - halfSize - eps) + 1.0f + halfSize + 0.001f;
            vel.x = 0;
        }

        pos.z += vel.z * stepDt;
        if (checkCollision(pos.x, pos.y, pos.z)) {
            if (vel.z > 0) pos.z = std::floor(pos.z + halfSize + eps) - halfSize - 0.001f;
            else pos.z = std::floor(pos.z - halfSize - eps) + 1.0f + halfSize + 0.001f;
            vel.z = 0;
        }
    }

    vel.x *= std::pow(0.5f, dt * 10); 
    vel.z *= std::pow(0.5f, dt * 10);
}