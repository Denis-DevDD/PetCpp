#pragma once

#include <nav/level.hpp>
#include <nav/mesh.hpp>
#include <optional>

namespace nav {

struct BuildSettings {
    float agentRadius = 0.f;
};

std::optional<NavMesh> build(const Level& level, const BuildSettings& settings = {});

}
