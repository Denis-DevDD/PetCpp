#include <doctest/doctest.h>
#include <nav/astar.hpp>
#include <nav/builder.hpp>

#include <algorithm>

using namespace nav;

TEST_CASE("astar: path exists across a simple mesh") {
    Level level;
    level.outer = {{0, 0}, {20, 0}, {20, 20}, {0, 20}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    auto corridor = findCorridor(*mesh, {1, 1}, {19, 19});
    REQUIRE_FALSE(corridor.empty());
    CHECK(corridor.front() == mesh->findTriangle({1, 1}));
    CHECK(corridor.back() == mesh->findTriangle({19, 19}));
}

TEST_CASE("astar: start and goal in the same triangle") {
    Level level;
    level.outer = {{0, 0}, {20, 0}, {20, 20}, {0, 20}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    TriId t = mesh->findTriangle({1, 1});
    REQUIRE(t != NoTri);

    auto corridor = findCorridor(*mesh, {1, 1}, {1.5f, 1.5f});
    REQUIRE(corridor.size() == 1);
    CHECK(corridor[0] == t);
}

TEST_CASE("astar: unreachable goal (disconnected mesh component) yields no corridor") {

    std::vector<Vec2> vertices = {
        {0, 0}, {10, 0}, {10, 10}, {0, 10},
        {100, 100}, {110, 100}, {110, 110}, {100, 110},
    };
    std::vector<std::array<int, 3>> triangles = {
        {0, 1, 2}, {0, 2, 3},
        {4, 5, 6}, {4, 6, 7},
    };

    NavMesh mesh = NavMesh::fromTriangleSoup(vertices, triangles);

    auto corridor = findCorridor(mesh, {1, 1}, {105, 105});
    CHECK(corridor.empty());
}

TEST_CASE("astar: point outside the mesh yields no corridor") {
    Level level;
    level.outer = {{0, 0}, {20, 0}, {20, 20}, {0, 20}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    auto corridor = findCorridor(*mesh, {1, 1}, {-50, -50});
    CHECK(corridor.empty());
}

TEST_CASE("astar: visitedOut collects every explored triangle, including the corridor") {
    Level level;
    level.outer = {{0, 0}, {30, 0}, {30, 30}, {0, 30}};
    level.holes = {{{10, 10}, {10, 20}, {20, 20}, {20, 10}}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    std::vector<TriId> visited;
    auto corridor = findCorridor(*mesh, {2, 15}, {28, 15}, &visited);
    REQUIRE_FALSE(corridor.empty());
    CHECK(visited.size() >= corridor.size());
    for (TriId t : corridor) {
        CHECK(std::find(visited.begin(), visited.end(), t) != visited.end());
    }
}

TEST_CASE("astar: corridor routes around a hole") {
    Level level;
    level.outer = {{0, 0}, {30, 0}, {30, 30}, {0, 30}};
    level.holes = {{{10, 10}, {10, 20}, {20, 20}, {20, 10}}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    auto corridor = findCorridor(*mesh, {2, 15}, {28, 15});
    REQUIRE_FALSE(corridor.empty());

    const auto& tris = mesh->triangles();
    for (size_t i = 0; i + 1 < corridor.size(); ++i) {
        const auto& tri = tris[static_cast<size_t>(corridor[i])];
        bool adjacent = tri.neighbor[0] == corridor[i + 1] || tri.neighbor[1] == corridor[i + 1] ||
                         tri.neighbor[2] == corridor[i + 1];
        CHECK(adjacent);
    }
}
