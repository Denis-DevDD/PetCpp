#include "draw.hpp"

#include <raylib.h>

namespace {

Vector2 toScreen(nav::Vec2 p) {
    return Vector2{p.x + kViewOffsetX, p.y + kViewOffsetY};
}

void drawPolygonOutline(const std::vector<nav::Vec2>& poly, Color color) {
    const size_t n = poly.size();
    for (size_t i = 0; i < n; ++i) {
        Vector2 a = toScreen(poly[i]);
        Vector2 b = toScreen(poly[(i + 1) % n]);
        DrawLineEx(a, b, 2.f, color);
    }
}

}

void drawLevel(const nav::Level& level) {
    drawPolygonOutline(level.outer, DARKGRAY);
    for (const auto& hole : level.holes) {
        drawPolygonOutline(hole, MAROON);
    }
}

void drawInsetOutline(const std::vector<nav::OffsetPolygon>& insetPolys) {
    for (const auto& poly : insetPolys) {
        drawPolygonOutline(poly.outer, BLUE);
        for (const auto& hole : poly.holes) {
            drawPolygonOutline(hole, ORANGE);
        }
    }
}

void drawMeshWireframe(const nav::NavMesh& mesh) {
    const auto& vs = mesh.vertices();
    for (const auto& tri : mesh.triangles()) {
        Vector2 a = toScreen(vs[tri.v[0]]);
        Vector2 b = toScreen(vs[tri.v[1]]);
        Vector2 c = toScreen(vs[tri.v[2]]);
        DrawLineEx(a, b, 1.f, SKYBLUE);
        DrawLineEx(b, c, 1.f, SKYBLUE);
        DrawLineEx(c, a, 1.f, SKYBLUE);
    }
}

void drawHighlightedTriangle(const nav::NavMesh& mesh, nav::TriId t) {
    if (t == nav::NoTri) return;
    const auto& vs = mesh.vertices();
    const auto& tri = mesh.triangles()[t];
    Vector2 a = toScreen(vs[tri.v[0]]);
    Vector2 b = toScreen(vs[tri.v[1]]);
    Vector2 c = toScreen(vs[tri.v[2]]);
    DrawTriangle(a, b, c, Fade(GREEN, 0.5f));
}

void drawVisited(const nav::NavMesh& mesh, const std::vector<nav::TriId>& visited) {
    const auto& vs = mesh.vertices();
    for (nav::TriId t : visited) {
        const auto& tri = mesh.triangles()[static_cast<size_t>(t)];
        Vector2 a = toScreen(vs[tri.v[0]]);
        Vector2 b = toScreen(vs[tri.v[1]]);
        Vector2 c = toScreen(vs[tri.v[2]]);
        DrawTriangle(a, b, c, Fade(YELLOW, 0.25f));
    }
}

void drawCorridor(const nav::NavMesh& mesh, const std::vector<nav::TriId>& corridor) {
    const auto& vs = mesh.vertices();
    for (nav::TriId t : corridor) {
        const auto& tri = mesh.triangles()[static_cast<size_t>(t)];
        Vector2 a = toScreen(vs[tri.v[0]]);
        Vector2 b = toScreen(vs[tri.v[1]]);
        Vector2 c = toScreen(vs[tri.v[2]]);
        DrawTriangle(a, b, c, Fade(ORANGE, 0.35f));
    }
}

void drawPortals(const nav::NavMesh& mesh, const std::vector<nav::TriId>& corridor) {
    for (size_t i = 0; i + 1 < corridor.size(); ++i) {
        nav::Vec2 left, right;
        mesh.portal(corridor[i], corridor[i + 1], left, right);
        DrawLineEx(toScreen(left), toScreen(right), 3.f, PURPLE);
    }
}

void drawMarker(nav::Vec2 p, Color color) {
    DrawCircleV(toScreen(p), 6.f, color);
}

void drawPath(const nav::Path& path) {
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        DrawLineEx(toScreen(path[i]), toScreen(path[i + 1]), 4.f, VIOLET);
    }
    for (const auto& p : path) {
        DrawCircleV(toScreen(p), 4.f, VIOLET);
    }
}
