#include "draw.hpp"
#include "ui.hpp"

#include <nav/builder.hpp>
#include <nav/level.hpp>
#include <nav/offset.hpp>
#include <nav/query.hpp>
#include <raylib.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <optional>
#include <vector>

namespace {
using Clock = std::chrono::steady_clock;
double millisSince(Clock::time_point t0) {
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}
}

int main(int argc, char** argv) {
    const char* levelPath = argc > 1 ? argv[1] : "data/levels/rooms.json";

    auto level = nav::loadLevel(levelPath);
    if (!level) {
        std::fprintf(stderr, "Failed to load level: %s\n", levelPath);
        return 1;
    }

    float agentRadius = 0.f;
    LayerToggles toggles;

    std::vector<nav::OffsetPolygon> insetPolys;
    std::optional<nav::NavMesh> mesh;
    double buildMs = 0.0;

    auto rebuildMesh = [&]() {
        auto t0 = Clock::now();
        insetPolys = nav::offsetLevel(*level, agentRadius);
        mesh = nav::build(*level, nav::BuildSettings{agentRadius});
        buildMs = millisSince(t0);
    };

    nav::Vec2 startPoint{20.f, 20.f};
    nav::Vec2 goalPoint{60.f, 60.f};
    std::vector<nav::TriId> corridor;
    std::vector<nav::TriId> visited;
    nav::Path path;
    double queryMicros = 0.0;

    auto recomputeCorridor = [&]() {
        corridor.clear();
        visited.clear();
        path.clear();
        if (!mesh) return;
        nav::NavQuery query(*mesh);
        auto t0 = Clock::now();
        path = query.findPath(startPoint, goalPoint);
        queryMicros = std::chrono::duration<double, std::micro>(Clock::now() - t0).count();
        corridor = query.lastCorridor();
        visited = query.lastVisited();
    };

    InitWindow(1280, 800, "Navmesh2D Viewer");
    SetTargetFPS(60);
    rebuildMesh();
    recomputeCorridor();

    bool draggingStart = false;
    bool draggingGoal = false;

    while (!WindowShouldClose()) {
        nav::Vec2 mouseWorld = screenToWorld(static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY()));
        bool mouseOverPanel = GetMouseX() > 1280 - 260;

        if (!mouseOverPanel) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) draggingStart = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) draggingGoal = true;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) draggingStart = false;
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) draggingGoal = false;

        if (draggingStart && (startPoint.x != mouseWorld.x || startPoint.y != mouseWorld.y)) {
            startPoint = mouseWorld;
            recomputeCorridor();
        }
        if (draggingGoal && (goalPoint.x != mouseWorld.x || goalPoint.y != mouseWorld.y)) {
            goalPoint = mouseWorld;
            recomputeCorridor();
        }

        if (IsKeyPressed(KEY_P)) TakeScreenshot("navmesh_shot.png");

        static bool cycling = false;
        static double cycleTimer = 0.0;
        static int cycleStage = 0;
        if (IsKeyPressed(KEY_C)) {
            cycling = !cycling;
            cycleStage = 0;
            cycleTimer = 0.0;
        }
        if (cycling) {
            cycleTimer += GetFrameTime();
            if (cycleTimer > 1.2) {
                cycleTimer = 0.0;
                cycleStage = (cycleStage + 1) % 7;
            }
            toggles.showInput = cycleStage >= 0;
            toggles.showInset = cycleStage >= 1;
            toggles.showTriangulation = cycleStage >= 2;
            toggles.showVisited = cycleStage >= 3;
            toggles.showCorridor = cycleStage >= 4;
            toggles.showPortals = cycleStage >= 5;
            toggles.showPath = cycleStage >= 6;
        }

        nav::TriId hovered = mesh ? mesh->findTriangle(mouseWorld) : nav::NoTri;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (toggles.showInput) drawLevel(*level);
        if (mesh) {
            if (toggles.showInset) drawInsetOutline(insetPolys);
            if (toggles.showTriangulation) drawMeshWireframe(*mesh);
            if (toggles.showVisited) drawVisited(*mesh, visited);
            if (toggles.showCorridor) drawCorridor(*mesh, corridor);
            if (toggles.showPortals) drawPortals(*mesh, corridor);
            if (toggles.showPath) drawPath(path);
            drawHighlightedTriangle(*mesh, hovered);
        }
        drawMarker(startPoint, LIME);
        drawMarker(goalPoint, RED);

        DrawText(levelPath, 10, 10, 20, GRAY);
        DrawText(TextFormat("build: %.3f ms   query: %.1f us", buildMs, queryMicros), 10, 35, 20, GRAY);
        DrawText("drag: L=start R=goal   P=screenshot   C=cycle layers", 10, 60, 20, GRAY);
        if (!mesh) {
            DrawText("no walkable area at this radius", 10, 85, 20, RED);
        } else if (corridor.empty()) {
            DrawText("no path (unreachable)", 10, 85, 20, RED);
        }

        float radiusBefore = agentRadius;
        drawControlPanel(agentRadius, toggles);
        if (agentRadius != radiusBefore) {
            rebuildMesh();
            recomputeCorridor();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
