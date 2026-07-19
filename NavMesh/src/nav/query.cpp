#include <nav/query.hpp>

#include "astar.hpp"
#include "funnel.hpp"
#include "geometry.hpp"

namespace nav {

Path NavQuery::findPath(Vec2 start, Vec2 goal) const {
    lastCorridor_ = findCorridor(mesh_, start, goal, &lastVisited_);
    if (lastCorridor_.empty()) return {};

    if (lastCorridor_.size() == 1) return {start, goal};

    std::vector<std::pair<Vec2, Vec2>> portals;
    portals.reserve(lastCorridor_.size() - 1);
    for (size_t i = 0; i + 1 < lastCorridor_.size(); ++i) {
        Vec2 left, right;
        mesh_.portal(lastCorridor_[i], lastCorridor_[i + 1], left, right);
        portals.emplace_back(left, right);
    }

    return funnel(start, goal, portals);
}

bool NavQuery::raycast(Vec2 from, Vec2 to) const {
    TriId cur = mesh_.findTriangle(from);
    TriId targetTri = mesh_.findTriangle(to);
    if (cur == NoTri || targetTri == NoTri) return false;
    if (cur == targetTri) return true;

    const auto& vs = mesh_.vertices();
    TriId prev = NoTri;

    for (size_t steps = 0, guard = mesh_.triangles().size() + 1; steps < guard; ++steps) {
        const Triangle& tri = mesh_.triangles()[static_cast<size_t>(cur)];
        TriId next = NoTri;

        for (int e = 0; e < 3; ++e) {
            if (tri.neighbor[e] == prev)
                continue;
            Vec2 a = vs[tri.v[e]];
            Vec2 b = vs[tri.v[(e + 1) % 3]];
            if (geom::segmentsIntersect(from, to, a, b)) {
                if (tri.neighbor[e] == NoTri)
                    return false;
                next = tri.neighbor[e];
                break;
            }
        }

        if (next == NoTri)
            return false;
        prev = cur;
        cur = next;
        if (cur == targetTri)
            return true;
    }
    return false;
}

}
