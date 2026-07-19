#include "funnel.hpp"

#include "geometry.hpp"

namespace nav {

namespace {

float triarea2(Vec2 a, Vec2 b, Vec2 c) {
    return geom::orientation(a, b, c);
}

}

Path funnel(Vec2 start, Vec2 goal, const std::vector<std::pair<Vec2, Vec2>>& portals) {
    std::vector<std::pair<Vec2, Vec2>> p;
    p.reserve(portals.size() + 2);
    p.emplace_back(start, start);
    p.insert(p.end(), portals.begin(), portals.end());
    p.emplace_back(goal, goal);

    Path path{start};

    Vec2 apex = start;
    Vec2 portalLeft = start;
    Vec2 portalRight = start;
    size_t apexIndex = 0, leftIndex = 0, rightIndex = 0;

    for (size_t i = 1; i < p.size(); ++i) {
        Vec2 left = p[i].first;
        Vec2 right = p[i].second;

        if (triarea2(apex, portalRight, right) <= 0.f) {

            if (apex == portalRight || triarea2(apex, portalLeft, right) > -geom::kEpsilon) {
                portalRight = right;
                rightIndex = i;
            } else {

                path.push_back(portalLeft);
                apex = portalLeft;
                apexIndex = leftIndex;
                portalLeft = apex;
                portalRight = apex;
                i = apexIndex;
                continue;
            }
        }

        if (triarea2(apex, portalLeft, left) >= 0.f) {
            if (apex == portalLeft || triarea2(apex, portalRight, left) < geom::kEpsilon) {
                portalLeft = left;
                leftIndex = i;
            } else {
                path.push_back(portalRight);
                apex = portalRight;
                apexIndex = rightIndex;
                portalLeft = apex;
                portalRight = apex;
                i = apexIndex;
                continue;
            }
        }
    }

    path.push_back(goal);
    return path;
}

}
