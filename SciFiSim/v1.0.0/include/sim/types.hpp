#pragma once
#include <cstdint>

namespace sim {

using Amount = std::int64_t;
using Tick = std::int64_t;

struct Vec2i {
    int x = 0;
    int y = 0;
    bool operator==(const Vec2i& o) const { return x == o.x && y == o.y; }
};

}