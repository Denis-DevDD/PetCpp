#include <nav/level.hpp>

#include "geometry.hpp"

#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

namespace nav {

namespace {

bool readPolygon(const nlohmann::json& arr, std::vector<Vec2>& out) {
    if (!arr.is_array()) return false;
    out.clear();
    out.reserve(arr.size());
    for (const auto& pt : arr) {
        if (!pt.is_array() || pt.size() != 2) return false;
        out.push_back(Vec2{pt[0].get<float>(), pt[1].get<float>()});
    }
    return out.size() >= 3;
}

}

std::optional<Level> loadLevel(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return std::nullopt;

    nlohmann::json j;
    try {
        file >> j;
    } catch (const nlohmann::json::exception&) {
        return std::nullopt;
    }

    if (!j.contains("outer")) return std::nullopt;

    Level level;
    if (!readPolygon(j["outer"], level.outer)) return std::nullopt;
    if (geom::signedArea2(level.outer) < 0.f) {
        std::reverse(level.outer.begin(), level.outer.end());
    }

    if (j.contains("holes")) {
        if (!j["holes"].is_array()) return std::nullopt;
        for (const auto& holeArr : j["holes"]) {
            std::vector<Vec2> hole;
            if (!readPolygon(holeArr, hole)) return std::nullopt;
            if (geom::signedArea2(hole) > 0.f) {
                std::reverse(hole.begin(), hole.end());
            }
            level.holes.push_back(std::move(hole));
        }
    }

    return level;
}

}
