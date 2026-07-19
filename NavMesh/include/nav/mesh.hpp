#pragma once

#include <nav/types.hpp>
#include <array>
#include <vector>

namespace nav {

struct Triangle {
    int v[3] = {0, 0, 0};
    int neighbor[3] = {NoTri, NoTri, NoTri};
};

class NavMesh {
public:
    NavMesh() = default;

    static NavMesh fromTriangleSoup(const std::vector<Vec2>& vertices,
        const std::vector<std::array<int, 3>>& triangles);

    const std::vector<Vec2>& vertices() const { return vertices_; }
    const std::vector<Triangle>& triangles() const { return triangles_; }

    TriId findTriangle(Vec2 p) const;

    void portal(TriId a, TriId b, Vec2& outLeft, Vec2& outRight) const;

    Vec2 centroid(TriId t) const;

private:
    std::vector<Vec2> vertices_;
    std::vector<Triangle> triangles_;

    struct Grid {
        float minX = 0.f, minY = 0.f, cellSize = 1.f;
        int cols = 1, rows = 1;
        std::vector<std::vector<TriId>> cells;
    };
    Grid grid_;

    void buildSpatialGrid();
};

}
