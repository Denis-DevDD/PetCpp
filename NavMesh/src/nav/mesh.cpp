#include "geometry.hpp"
#include <nav/mesh.hpp>

#include <algorithm>
#include <cmath>
#include <map>

namespace nav {

namespace {

constexpr float kWeldEpsilon = 1e-3f;

int weldedIndex(std::vector<Vec2>& welded, Vec2 p) {
    for (size_t i = 0; i < welded.size(); ++i) {
        if (lengthSq(welded[i] - p) < kWeldEpsilon * kWeldEpsilon) return static_cast<int>(i);
    }
    welded.push_back(p);
    return static_cast<int>(welded.size() - 1);
}

}

NavMesh NavMesh::fromTriangleSoup(const std::vector<Vec2>& vertices,
        const std::vector<std::array<int, 3>>& triangles) {
    NavMesh mesh;

    std::vector<int> remap(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        remap[i] = weldedIndex(mesh.vertices_, vertices[i]);
    }

    std::vector<std::array<int, 3>> origTris;
    mesh.triangles_.reserve(triangles.size());
    origTris.reserve(triangles.size());
    for (const auto& t : triangles) {
        Triangle tri;
        tri.v[0] = remap[t[0]];
        tri.v[1] = remap[t[1]];
        tri.v[2] = remap[t[2]];

        mesh.triangles_.push_back(tri);
        origTris.push_back({t[0], t[1], t[2]});
    }

    std::map<std::pair<int, int>, std::pair<TriId, int>> edgeOwner;
    for (size_t ti = 0; ti < origTris.size(); ++ti) {
        for (int e = 0; e < 3; ++e) {
            int a = origTris[ti][e];
            int b = origTris[ti][(e + 1) % 3];
            auto key = std::minmax(a, b);
            auto it = edgeOwner.find(key);
            if (it == edgeOwner.end()) {
                edgeOwner[key] = {static_cast<TriId>(ti), e};
            } else {
                auto [otherTri, otherEdge] = it->second;
                mesh.triangles_[ti].neighbor[e] = otherTri;
                mesh.triangles_[otherTri].neighbor[otherEdge] = static_cast<TriId>(ti);
            }
        }
    }

    mesh.buildSpatialGrid();
    return mesh;
}

void NavMesh::buildSpatialGrid() {
    if (vertices_.empty() || triangles_.empty()) {
        grid_ = Grid{};
        return;
    }

    float minX = vertices_[0].x, maxX = vertices_[0].x;
    float minY = vertices_[0].y, maxY = vertices_[0].y;
    for (const auto& v : vertices_) {
        minX = std::min(minX, v.x);
        maxX = std::max(maxX, v.x);
        minY = std::min(minY, v.y);
        maxY = std::max(maxY, v.y);
    }

    int targetCells = std::clamp(static_cast<int>(std::sqrt(triangles_.size())), 1, 128);
    float width = std::max(maxX - minX, 1.f);
    float height = std::max(maxY - minY, 1.f);
    float cellSize = std::max(width, height) / static_cast<float>(targetCells);
    cellSize = std::max(cellSize, 1e-3f);

    grid_.minX = minX;
    grid_.minY = minY;
    grid_.cellSize = cellSize;
    grid_.cols = std::max(1, static_cast<int>(width / cellSize) + 1);
    grid_.rows = std::max(1, static_cast<int>(height / cellSize) + 1);
    grid_.cells.assign(static_cast<size_t>(grid_.cols) * grid_.rows, {});

    auto cellOf = [&](float x, float y) {
        int cx = std::clamp(static_cast<int>((x - grid_.minX) / grid_.cellSize), 0, grid_.cols - 1);
        int cy = std::clamp(static_cast<int>((y - grid_.minY) / grid_.cellSize), 0, grid_.rows - 1);
        return std::pair{cx, cy};
    };

    for (size_t ti = 0; ti < triangles_.size(); ++ti) {
        const Triangle& tri = triangles_[ti];
        Vec2 a = vertices_[tri.v[0]], b = vertices_[tri.v[1]], c = vertices_[tri.v[2]];
        float triMinX = std::min({a.x, b.x, c.x}), triMaxX = std::max({a.x, b.x, c.x});
        float triMinY = std::min({a.y, b.y, c.y}), triMaxY = std::max({a.y, b.y, c.y});

        auto [cx0, cy0] = cellOf(triMinX, triMinY);
        auto [cx1, cy1] = cellOf(triMaxX, triMaxY);
        for (int cy = cy0; cy <= cy1; ++cy) {
            for (int cx = cx0; cx <= cx1; ++cx) {
                grid_.cells[static_cast<size_t>(cy) * grid_.cols + cx].push_back(static_cast<TriId>(ti));
            }
        }
    }
}

TriId NavMesh::findTriangle(Vec2 p) const {
    if (grid_.cells.empty()) return NoTri;

    int cx = static_cast<int>((p.x - grid_.minX) / grid_.cellSize);
    int cy = static_cast<int>((p.y - grid_.minY) / grid_.cellSize);
    if (cx < 0 || cx >= grid_.cols || cy < 0 || cy >= grid_.rows) return NoTri;

    for (TriId t : grid_.cells[static_cast<size_t>(cy) * grid_.cols + cx]) {
        const Triangle& tri = triangles_[t];
        if (geom::pointInTriangle(p, vertices_[tri.v[0]], vertices_[tri.v[1]], vertices_[tri.v[2]])) {
            return t;
        }
    }
    return NoTri;
}

void NavMesh::portal(TriId a, TriId b, Vec2& outLeft, Vec2& outRight) const {
    const Triangle& tri = triangles_[a];
    for (int e = 0; e < 3; ++e) {
        if (tri.neighbor[e] == b) {
            outLeft = vertices_[tri.v[e]];
            outRight = vertices_[tri.v[(e + 1) % 3]];
            return;
        }
    }
}

Vec2 NavMesh::centroid(TriId t) const {
    const Triangle& tri = triangles_[t];
    Vec2 a = vertices_[tri.v[0]], b = vertices_[tri.v[1]], c = vertices_[tri.v[2]];
    return Vec2{(a.x + b.x + c.x) / 3.f, (a.y + b.y + c.y) / 3.f};
}

}
