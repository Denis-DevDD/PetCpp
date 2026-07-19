#pragma once
#include "types.hpp"
#include <string>

namespace sim {

struct LoadResult {
    bool ok = false;
    std::string error;
    ContentDB db;
    PopConfig popCfg;
};

LoadResult loadContent(const std::string& path);

}