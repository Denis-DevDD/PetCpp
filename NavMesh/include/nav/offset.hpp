#pragma once

#include <nav/level.hpp>
#include <vector>

namespace nav {

struct OffsetPolygon {
    std::vector<Vec2> outer;
    std::vector<std::vector<Vec2>> holes;
};

std::vector<OffsetPolygon> offsetLevel(const Level& level, float radius);

}
