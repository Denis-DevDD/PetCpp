#pragma once

#include <nav/types.hpp>
#include <utility>
#include <vector>

namespace nav {

Path funnel(Vec2 start, Vec2 goal, const std::vector<std::pair<Vec2, Vec2>>& portals);

}
