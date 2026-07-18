#include "raylib.h"
#include "sim/world.hpp"
#include <algorithm>

int main() {
    const int screenW = 1280;
    const int screenH = 720;

    InitWindow(screenW, screenH, "SciFiSim");
    SetTargetFPS(60);

    sim::World world(20, 15);

    const int tileSize = 36;
    const int originX = 40;
    const int originY = 100;

    float acc = 0.0f;
    const float secondsPerTick = 1.0f;

    bool dragging = false;
    sim::Vec2i dragStart { 0, 0 };

    while (!WindowShouldClose()) {
        acc += GetFrameTime();
        while (acc >= secondsPerTick) {
            world.step();
            acc -= secondsPerTick;
        }

        Vector2 mouse = GetMousePosition();

        int cellX = (int)((mouse.x - originX) / tileSize);
        int cellY = (int)((mouse.y - originY) / tileSize);
        sim::Vec2i hovered {cellX, cellY};
        
        bool hoverValid = world.ifInBounds(hovered);

        if (mouse.x < originX || mouse.y < originY)
            hoverValid = false;

        if (hoverValid && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            dragging = true;
            dragStart = hovered;
        }

        if (dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            int x0 = std::min(dragStart.x, hovered.x);
            int x1 = std::max(dragStart.x, hovered.x);
            int y0 = std::min(dragStart.y, hovered.y);
            int y1 = std::max(dragStart.y, hovered.y);

            for (int y = y0; y <= y1; ++y)
                for (int x = x0; x <= x1; ++x)
                    world.placeBuilding({ x, y });   // сам отсеет занятые/вне границ

            dragging = false;
        }

        if (hoverValid && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            world.removeBuilding(hovered);
        }

        BeginDrawing();
        ClearBackground(Color{ 18, 18, 24, 255 });

        DrawText("SciFiSim", 20, 20, 40, RAYWHITE);
        DrawText(TextFormat("tick: %lld", (long long)world.getTick()),
                 20, 66, 22, Color{ 120, 200, 255, 255 });

        for (int y = 0; y < world.getHeight(); ++y) {
            for (int x = 0; x < world.getWidth(); ++x) {
                int px = originX + x * tileSize;
                int py = originY + y * tileSize;

                if (world.at({ x, y }).occupied) {
                    DrawRectangle(px, py, tileSize, tileSize,
                                  Color{ 90, 170, 120, 255 });
                }
                
                DrawRectangleLines(px, py, tileSize, tileSize,
                                   Color{ 45, 45, 60, 255 });
            }
        }

        if (dragging) {
            int x0 = std::min(dragStart.x, hovered.x);
            int x1 = std::max(dragStart.x, hovered.x);
            int y0 = std::min(dragStart.y, hovered.y);
            int y1 = std::max(dragStart.y, hovered.y);

            x0 = std::max(x0, 0);
            y0 = std::max(y0, 0);
            x1 = std::min(x1, world.getWidth()  - 1);
            y1 = std::min(y1, world.getHeight() - 1);

            for (int y = y0; y <= y1; ++y) {
                for (int x = x0; x <= x1; ++x) {
                    int px = originX + x * tileSize;
                    int py = originY + y * tileSize;
                    bool free = !world.at({ x, y }).occupied;
                    Color c = free ? Color{ 120, 200, 255, 70 }
                                   : Color{ 220, 100, 100, 70 };
                    DrawRectangle(px, py, tileSize, tileSize, c);
                }
            }

            int rx = originX + x0 * tileSize;
            int ry = originY + y0 * tileSize;
            int rw = (x1 - x0 + 1) * tileSize;
            int rh = (y1 - y0 + 1) * tileSize;
            DrawRectangleLines(rx, ry, rw, rh, Color{ 120, 200, 255, 255 });
            DrawText(TextFormat("%d x %d", x1 - x0 + 1, y1 - y0 + 1),
                     rx + 4, ry + 4, 18, RAYWHITE);
        }

        if (hoverValid && !dragging) {
            int px = originX + hovered.x * tileSize;
            int py = originY + hovered.y * tileSize;
            
            DrawRectangle(px, py, tileSize, tileSize,
                          Color{ 120, 200, 255, 40 });
            
            DrawRectangleLines(px, py, tileSize, tileSize,
                               Color{ 120, 200, 255, 255 });
        }

        DrawText(TextFormat("cell: %d, %d  |  LMB: build   RMB: remove", hovered.x, hovered.y),
            20, screenH - 34, 20, Color{ 150, 150, 160, 255 });

        DrawFPS(screenW - 90, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}