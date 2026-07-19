#pragma once

#include <nav/mesh.hpp>
#include <vector>

namespace nav {

std::vector<TriId> findCorridor(const NavMesh& mesh, Vec2 start, Vec2 goal,
    std::vector<TriId>* visitedOut = nullptr);

}
