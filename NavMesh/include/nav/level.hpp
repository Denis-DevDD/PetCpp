#pragma once

#include <nav/types.hpp>
#include <optional>
#include <string>
#include <vector>

namespace nav {

struct Level {
    std::vector<Vec2> outer;
    std::vector<std::vector<Vec2>> holes;
};

std::optional<Level> loadLevel(const std::string& path);

}
