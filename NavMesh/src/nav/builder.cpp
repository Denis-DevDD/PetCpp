#include <nav/builder.hpp>
#include <nav/offset.hpp>

#include "triangulate.hpp"

namespace nav {

std::optional<NavMesh> build(const Level& level, const BuildSettings& settings) {
    std::vector<OffsetPolygon> insetPolys = offsetLevel(level, settings.agentRadius);
    if (insetPolys.empty()) return std::nullopt;

    std::vector<Vec2> vertices;
    std::vector<std::array<int, 3>> triangles;

    for (const auto& poly : insetPolys) {
        std::vector<Vec2> merged = bridgeHoles(poly.outer, poly.holes);
        std::vector<RawTriangle> tris = triangulateSimplePolygon(merged);
        if (tris.empty()) continue;

        int offset = static_cast<int>(vertices.size());
        vertices.insert(vertices.end(), merged.begin(), merged.end());
        for (const auto& t : tris) {
            triangles.push_back({t.v[0] + offset, t.v[1] + offset, t.v[2] + offset});
        }
    }

    if (triangles.empty()) return std::nullopt;

    return NavMesh::fromTriangleSoup(vertices, triangles);
}

}
