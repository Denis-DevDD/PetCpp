#include <doctest/doctest.h>
#include <nav/level.hpp>
#include <nav/geometry.hpp>

using namespace nav;

TEST_CASE("orientation sign correctness") {
    Vec2 a{0, 0}, b{1, 0}, cLeft{0, 1}, cRight{0, -1}, cCollinear{2, 0};

    CHECK(geom::orientation(a, b, cLeft) > 0.f);
    CHECK(geom::orientation(a, b, cRight) < 0.f);
    CHECK(geom::orientation(a, b, cCollinear) == doctest::Approx(0.f));
}

TEST_CASE("point in triangle: inside, outside, on edge") {
    Vec2 a{0, 0}, b{4, 0}, c{0, 4};

    CHECK(geom::pointInTriangle({1, 1}, a, b, c));
    CHECK_FALSE(geom::pointInTriangle({3, 3}, a, b, c));
    CHECK(geom::pointInTriangle({2, 0}, a, b, c));
    CHECK(geom::pointInTriangle({0, 0}, a, b, c));

    CHECK(geom::pointInTriangle({1, 1}, b, a, c));
    CHECK_FALSE(geom::pointInTriangle({3, 3}, b, a, c));
}

TEST_CASE("segment intersection: true and false cases") {
    CHECK(geom::segmentsIntersect({0, 0}, {4, 4}, {0, 4}, {4, 0}));
    CHECK_FALSE(geom::segmentsIntersect({0, 0}, {1, 1}, {5, 5}, {6, 6}));
    CHECK(geom::segmentsIntersect({0, 0}, {2, 2}, {2, 2}, {4, 0}));
    CHECK(geom::segmentsIntersect({0, 0}, {4, 0}, {2, 0}, {6, 0}));
    CHECK_FALSE(geom::segmentsIntersect({0, 0}, {2, 0}, {3, 0}, {5, 0}));
    CHECK_FALSE(geom::segmentsIntersect({0, 0}, {4, 0}, {0, 1}, {4, 1}));
}

TEST_CASE("signed area sign matches winding") {
    std::vector<Vec2> ccw = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    std::vector<Vec2> cw = {{0, 0}, {0, 4}, {4, 4}, {4, 0}};

    CHECK(geom::signedArea2(ccw) > 0.f);
    CHECK(geom::signedArea2(cw) < 0.f);
    CHECK(std::abs(geom::signedArea2(ccw)) == doctest::Approx(32.f));
}

TEST_CASE("level loader normalizes winding") {
    auto level = loadLevel("data/levels/rooms.json");
    REQUIRE(level.has_value());
    CHECK(geom::signedArea2(level->outer) > 0.f);
    REQUIRE(level->holes.size() == 1);
    CHECK(geom::signedArea2(level->holes[0]) < 0.f);
}

TEST_CASE("level loader rejects missing file") {
    auto level = loadLevel("data/levels/does_not_exist.json");
    CHECK_FALSE(level.has_value());
}
