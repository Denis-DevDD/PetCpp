#include <raylib.h>

int main() {
    InitWindow(1280, 800, "Navmesh2D Viewer");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
