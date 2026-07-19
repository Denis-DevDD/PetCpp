#pragma once

#include <nav/types.hpp>
#include <vector>

namespace nav {

struct RawTriangle {
    int v[3];
};

std::vector<RawTriangle> triangulateSimplePolygon(const std::vector<Vec2>& poly);

std::vector<Vec2> bridgeHoles(const std::vector<Vec2>& outer, const std::vector<std::vector<Vec2>>& holes);

}
