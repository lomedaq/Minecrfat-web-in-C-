#pragma once
#include "raylib.h"
#include "raymath.h"

#include "world/world.h"

class GameCamera {
private:
    Camera3D camera;

    float yaw   = 0.0f;
    float pitch = 0.0f;

    float sensitivity = 0.003f;
    float speed = 5.0f;

public:
    Vector3 forward = {0.0f, 0.0f, 0.0f};
    Vector3 right = {0.0f, 0.0f, 0.0f};

    inline GameCamera() {
        camera = {0};
        camera.position = {-2, 10, -2};
        camera.target   = {0, 0, 0};
        camera.up       = {0,1,0};
        camera.fovy     = 90.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        DisableCursor();
    }

    Camera3D GetCamera() { return camera; }

    void Update(Vector3 playerPos, float dt) {
        Vector2 delta = GetMouseDelta();
        yaw -= delta.x * sensitivity;
        pitch -= delta.y * sensitivity;

        if (pitch > 1.5f) pitch = 1.5f;
        if (pitch < -1.5f) pitch = -1.5f;

        forward = {
            cosf(pitch) * sinf(yaw),
            sinf(pitch),
            cosf(pitch) * cosf(yaw)
        };

        right = {
            sinf(yaw - PI/2.0f),
            0,
            cosf(yaw - PI/2.0f)
        };

        // camera.position = playerPos + (Vector3){0, 1.5f, 0};
        camera.position = playerPos;
        camera.target = Vector3Add(camera.position, forward);
    }
};