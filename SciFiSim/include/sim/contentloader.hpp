#pragma once
#include "types.hpp"
#include <string>
#include <vector>

namespace sim {

struct LoadResult {
    bool ok = false;
    std::string error;
    ContentDB db;
    PopConfig popCfg;
    std::vector<RecipeInput> startStock;
};

LoadResult loadContent(const std::string& path);

}