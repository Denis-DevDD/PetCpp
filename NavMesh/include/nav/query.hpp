#pragma once

#include <nav/mesh.hpp>
#include <nav/types.hpp>
#include <vector>

namespace nav {

class NavQuery {
public:
    explicit NavQuery(const NavMesh& mesh) : mesh_(mesh) {}

    Path findPath(Vec2 start, Vec2 goal) const;

    const std::vector<TriId>& lastCorridor() const { return lastCorridor_; }

    const std::vector<TriId>& lastVisited() const { return lastVisited_; }

    bool raycast(Vec2 from, Vec2 to) const;

private:
    const NavMesh& mesh_;
    mutable std::vector<TriId> lastCorridor_;
    mutable std::vector<TriId> lastVisited_;
};

}
