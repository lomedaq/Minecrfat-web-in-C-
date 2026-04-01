#include "raylib.h"
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif
#include "world/world.h"

void UpdateDrawFrame(void);

World* worldPtr = nullptr;

int main(void) {
#if defined(PLATFORM_WEB)
    InitWindow(1280, 720, "Voxel Chunk - Web");
#else
    int monitor = GetCurrentMonitor();
    InitWindow(GetMonitorWidth(monitor), GetMonitorHeight(monitor), "Voxel Chunk");
    ToggleFullscreen();
#endif
    DisableCursor();

    worldPtr = new World();
    worldPtr->generateWorld();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    worldPtr->dispose();
    delete worldPtr;
    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void) {
    worldPtr->update(GetFrameTime());

    BeginDrawing();
    ClearBackground(RAYWHITE);
    worldPtr->render();
    DrawFPS(10, 10);
    EndDrawing();
}