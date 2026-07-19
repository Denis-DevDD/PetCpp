#pragma once

#include <nav/level.hpp>
#include <nav/mesh.hpp>
#include <nav/offset.hpp>
#include <raylib.h>

constexpr float kViewOffsetX = 40.f;
constexpr float kViewOffsetY = 40.f;

inline nav::Vec2 screenToWorld(float screenX, float screenY) {
    return nav::Vec2{screenX - kViewOffsetX, screenY - kViewOffsetY};
}

void drawLevel(const nav::Level& level);

void drawInsetOutline(const std::vector<nav::OffsetPolygon>& insetPolys);

void drawMeshWireframe(const nav::NavMesh& mesh);

void drawHighlightedTriangle(const nav::NavMesh& mesh, nav::TriId t);

void drawVisited(const nav::NavMesh& mesh, const std::vector<nav::TriId>& visited);

void drawCorridor(const nav::NavMesh& mesh, const std::vector<nav::TriId>& corridor);

void drawPortals(const nav::NavMesh& mesh, const std::vector<nav::TriId>& corridor);

void drawPath(const nav::Path& path);

void drawMarker(nav::Vec2 p, Color color);
