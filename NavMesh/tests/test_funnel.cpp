#include <doctest/doctest.h>
#include <nav/builder.hpp>
#include <nav/funnel.hpp>
#include <nav/geometry.hpp>
#include <nav/query.hpp>

using namespace nav;

namespace {

bool crossesAnyEdge(Vec2 a, Vec2 b, const std::vector<Vec2>& contour) {
    const size_t n = contour.size();
    for (size_t i = 0; i < n; ++i) {
        Vec2 ea = contour[i], eb = contour[(i + 1) % n];
        if (a == ea || a == eb || b == ea || b == eb) continue;
        if (geom::segmentsIntersect(a, b, ea, eb)) return true;
    }
    return false;
}

}

TEST_CASE("funnel: straight open corridor yields a 2-point path") {

    std::vector<std::pair<Vec2, Vec2>> portals = {
        {{10, 10}, {10, 0}},
        {{20, 10}, {20, 0}},
    };

    Path path = funnel({1, 5}, {29, 5}, portals);
    CHECK(path.size() == 2);
    CHECK(path.front() == Vec2{1, 5});
    CHECK(path.back() == Vec2{29, 5});
}

TEST_CASE("funnel: no portals (same triangle) yields start and goal only") {
    Path path = funnel({3, 4}, {9, 2}, {});
    CHECK(path.size() == 2);
}

TEST_CASE("funnel: a corner in the corridor produces a bend at the corner vertex") {

    std::vector<std::pair<Vec2, Vec2>> portals = {
        {{0, 2}, {2, 2}},
    };

    Path path = funnel({3.9f, 0.5f}, {0.5f, 3.9f}, portals);
    REQUIRE(path.size() == 3);
    CHECK(path[1] == Vec2{0, 2});
}

TEST_CASE("funnel: path stays within the portal corridor bounds") {
    std::vector<std::pair<Vec2, Vec2>> portals = {
        {{0, 2}, {2, 2}},
    };
    Path path = funnel({3.9f, 0.5f}, {0.5f, 3.9f}, portals);

    for (const auto& p : path) {
        CHECK(p.x >= 0.f);
        CHECK(p.x <= 4.f);
        CHECK(p.y >= 0.f);
        CHECK(p.y <= 4.f);
    }
}

TEST_CASE("findPath: full pipeline produces a taut path that never crosses the hole") {
    Level level;
    level.outer = {{0, 0}, {30, 0}, {30, 30}, {0, 30}};
    level.holes = {{{10, 10}, {10, 20}, {20, 20}, {20, 10}}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    NavQuery query(*mesh);
    Vec2 start{2, 15};
    Vec2 goal{28, 15};
    Path path = query.findPath(start, goal);

    REQUIRE(path.size() >= 2);
    CHECK(path.front() == start);
    CHECK(path.back() == goal);

    for (size_t i = 0; i + 1 < path.size(); ++i) {
        CHECK_FALSE(crossesAnyEdge(path[i], path[i + 1], level.holes[0]));
    }
}

TEST_CASE("findPath: straight open level yields a direct 2-point path") {
    Level level;
    level.outer = {{0, 0}, {30, 0}, {30, 30}, {0, 30}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    NavQuery query(*mesh);
    Path path = query.findPath({2, 2}, {28, 28});
    CHECK(path.size() == 2);
}

TEST_CASE("findPath: takes the short way past an obstacle, not a detour") {

    Level level;
    level.outer = {{0, 0}, {400, 0}, {400, 400}, {0, 400}};
    level.holes = {{{150, 150}, {150, 250}, {250, 250}, {250, 150}}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    Vec2 start{200, 60}, goal{340, 200};
    NavQuery query(*mesh);
    Path path = query.findPath(start, goal);
    REQUIRE(path.size() >= 2);

    float pathLen = 0.f;
    for (size_t i = 0; i + 1 < path.size(); ++i) pathLen += length(path[i + 1] - path[i]);
    float straight = length(goal - start);

    CHECK(pathLen == doctest::Approx(straight).epsilon(0.02));
}

TEST_CASE("findPath: unreachable goal returns an empty path") {
    std::vector<Vec2> vertices = {
        {0, 0}, {10, 0}, {10, 10}, {0, 10},
        {100, 100}, {110, 100}, {110, 110}, {100, 110},
    };
    std::vector<std::array<int, 3>> triangles = {
        {0, 1, 2}, {0, 2, 3},
        {4, 5, 6}, {4, 6, 7},
    };
    NavMesh mesh = NavMesh::fromTriangleSoup(vertices, triangles);

    NavQuery query(mesh);
    Path path = query.findPath({1, 1}, {105, 105});
    CHECK(path.empty());
}

TEST_CASE("determinism: same query run twice yields an identical path") {
    Level level;
    level.outer = {{0, 0}, {30, 0}, {30, 30}, {0, 30}};
    level.holes = {{{10, 10}, {10, 20}, {20, 20}, {20, 10}}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    NavQuery q1(*mesh);
    NavQuery q2(*mesh);
    Path p1 = q1.findPath({2, 15}, {28, 15});
    Path p2 = q2.findPath({2, 15}, {28, 15});

    REQUIRE(p1.size() == p2.size());
    for (size_t i = 0; i < p1.size(); ++i) {
        CHECK(p1[i].x == p2[i].x);
        CHECK(p1[i].y == p2[i].y);
    }
}
