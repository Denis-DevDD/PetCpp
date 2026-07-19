#include "raylib.h"
#include "sim/world.hpp"
#include "sim/contentloader.hpp"
#include <algorithm>
#include <filesystem>
#include <string>

static Color toRay(sim::RGB c) {
    return Color{ c.r, c.g, c.b, 255 };
}

int main() {
    const int screenW = 1280;
    const int screenH = 720;

    InitWindow(screenW, screenH, "SciFiSim");
    SetTargetFPS(60);

    const std::string contentPath = "data/content.json";

    sim::LoadResult loaded = sim::loadContent(contentPath);
    
    if (!loaded.ok) {
        TraceLog(LOG_ERROR, "content load failed: %s", loaded.error.c_str());
        CloseWindow();
        return 1;
    }

    sim::ContentDB db = std::move(loaded.db);
    sim::PopConfig popCfg = loaded.popCfg;

    sim::BuildingId selected = 0;

    sim::World world(20, 15, db.resourceCount(), popCfg);

    std::filesystem::file_time_type lastWrite{};

    try {
        lastWrite = std::filesystem::last_write_time(contentPath);
    } catch (...) {}

    float reloadCheckAcc = 0.0f;
    const float reloadCheckInterval = 0.5f;
    std::string reloadMsg;
    float reloadMsgTimer = 0.0f; 

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
            world.step(db);
            acc -= secondsPerTick;
        }

        reloadCheckAcc += GetFrameTime();
        if (reloadMsgTimer > 0.0f) reloadMsgTimer -= GetFrameTime();

        if (reloadCheckAcc >= reloadCheckInterval) {
            reloadCheckAcc = 0.0f;
            std::filesystem::file_time_type current{};
            bool haveTime = false;
            try { current = std::filesystem::last_write_time(contentPath); haveTime = true; }
            catch (...) {}

            if (haveTime && current != lastWrite) {
                lastWrite = current;
                sim::LoadResult r = sim::loadContent(contentPath);
                if (r.ok) {
                    db = std::move(r.db);
                    popCfg = r.popCfg;
                    world.setPopConfig(popCfg);
                    if (selected >= db.count()) selected = 0;
                    reloadMsg = "content reloaded";
                } else {
                    reloadMsg = "reload FAILED: " + r.error;
                }
                reloadMsgTimer = 3.0f;
            }
        }

        if (IsKeyPressed(KEY_ONE)  && db.valid(0)) selected = 0;
        if (IsKeyPressed(KEY_TWO)  && db.valid(1)) selected = 1;


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
                    world.placeBuilding({ x, y }, selected);

            dragging = false;
        }

        if (hoverValid && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            world.removeBuilding(hovered);
        }

        BeginDrawing();
        ClearBackground(Color{ 18, 18, 24, 255 });

        DrawText("SciFiSim", 20, 20, 40, RAYWHITE);
        DrawText(TextFormat("prod x%.2f", world.getProductionMultiplierMilli() / 1000.0f),
            105, 66, 20, Color{ 200, 200, 140, 255 });
        DrawText(TextFormat("tick: %lld", (long long)world.getTick()),
            20, 66, 22, Color{ 120, 200, 255, 255 });


        DrawText(TextFormat("selected [%d]: %s", selected + 1, db.get(selected).name.c_str()),
            300, 66, 22, toRay(db.get(selected).color));


        {
            int panelX = originX + world.getWidth() * tileSize + 30;  // 30px отступ от сетки
            int panelY = originY;                                     // на уровне верха сетки
            const int rowH = 30;                                      // высота строки

            DrawText("RESOURCES", panelX, panelY, 20, Color{ 200, 200, 210, 255 });
            panelY += rowH + 6;

            for (int i = 0; i < db.resourceCount(); ++i) {
                const sim::ResourceDef& rd = db.getResource(i);
                DrawText(TextFormat("%s: %lld", rd.name.c_str(),
                                    (long long)world.stock(i)),
                         panelX, panelY, 22, toRay(rd.color));
                panelY += rowH;
            }
        }

        {
            int tx = 620;
            for (int i = 0; i < world.getTierCount(); ++i) {
                const sim::TierState& ts = world.getTier(i);
                const sim::TierDef& td = popCfg.tiers[static_cast<size_t>(i)];
                DrawText(TextFormat("%s: %lld (%d%%)",
                                    td.name.c_str(),
                                    (long long)ts.count, ts.satisfaction),
                         tx, 66, 22,
                         ts.satisfaction >= 50
                             ? Color{ 120, 220, 130, 255 }
                             : Color{ 230, 120, 120, 255 });
                tx += 220;
            }
        }

        for (int y = 0; y < world.getHeight(); ++y) {
            for (int x = 0; x < world.getWidth(); ++x) {
                int px = originX + x * tileSize;
                int py = originY + y * tileSize;

                const sim::Tile& t = world.at({ x, y });
                if (t.occupied()) {
                    DrawRectangle(px, py, tileSize, tileSize,
                        toRay(db.get(t.building).color));
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
                    bool free = !world.at({ x, y }).occupied();
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