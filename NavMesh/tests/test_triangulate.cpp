#include <doctest/doctest.h>
#include <nav/geometry.hpp>
#include <nav/level.hpp>
#include <nav/offset.hpp>
#include <nav/triangulate.hpp>

#include <cmath>

using namespace nav;

namespace {

float triangleArea(Vec2 a, Vec2 b, Vec2 c) {
    return std::abs(geom::orientation(a, b, c)) * 0.5f;
}

bool properlyCross(Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
    const float e = geom::kEpsilon;
    float d1 = geom::orientation(c, d, a);
    float d2 = geom::orientation(c, d, b);
    float d3 = geom::orientation(a, b, c);
    float d4 = geom::orientation(a, b, d);
    bool cd = (d1 > e && d2 < -e) || (d1 < -e && d2 > e);
    bool ab = (d3 > e && d4 < -e) || (d3 < -e && d4 > e);
    return cd && ab;
}

bool anyTriangleCrossesBoundary(const std::vector<Vec2>& poly,
                                 const std::vector<RawTriangle>& tris) {
    const size_t n = poly.size();
    for (const auto& t : tris) {
        for (int e = 0; e < 3; ++e) {
            Vec2 ta = poly[t.v[e]];
            Vec2 tb = poly[t.v[(e + 1) % 3]];
            for (size_t b = 0; b < n; ++b) {
                if (properlyCross(ta, tb, poly[b], poly[(b + 1) % n])) return true;
            }
        }
    }
    return false;
}

}

TEST_CASE("triangulate: square, count and area sum") {
    std::vector<Vec2> square = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};

    auto tris = triangulateSimplePolygon(square);
    CHECK(tris.size() == square.size() - 2);

    float total = 0.f;
    for (const auto& t : tris) {
        float a = triangleArea(square[t.v[0]], square[t.v[1]], square[t.v[2]]);
        CHECK(a > 1e-5f);
        total += a;
    }
    CHECK(total == doctest::Approx(std::abs(geom::signedArea2(square)) * 0.5f));
    CHECK_FALSE(anyTriangleCrossesBoundary(square, tris));
}

TEST_CASE("triangulate: concave L-shape") {
    std::vector<Vec2> lshape = {{0, 0}, {4, 0}, {4, 2}, {2, 2}, {2, 4}, {0, 4}};
    REQUIRE(geom::signedArea2(lshape) > 0.f);

    auto tris = triangulateSimplePolygon(lshape);
    CHECK(tris.size() == lshape.size() - 2);

    float total = 0.f;
    for (const auto& t : tris) {
        float a = triangleArea(lshape[t.v[0]], lshape[t.v[1]], lshape[t.v[2]]);
        CHECK(a > 1e-5f);
        total += a;
    }
    CHECK(total == doctest::Approx(std::abs(geom::signedArea2(lshape)) * 0.5f));
    CHECK_FALSE(anyTriangleCrossesBoundary(lshape, tris));
}

TEST_CASE("triangulate: degenerate input returns no triangles") {
    std::vector<Vec2> line = {{0, 0}, {1, 0}};
    CHECK(triangulateSimplePolygon(line).empty());
}

TEST_CASE("bridgeHoles: square with one hole triangulates to outer-minus-hole area") {
    std::vector<Vec2> outer = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    std::vector<Vec2> hole = {{4, 4}, {4, 3}, {3, 3}, {3, 4}};

    auto merged = bridgeHoles(outer, {hole});
    auto tris = triangulateSimplePolygon(merged);
    REQUIRE_FALSE(tris.empty());

    float total = 0.f;
    for (const auto& t : tris) {
        float a = triangleArea(merged[t.v[0]], merged[t.v[1]], merged[t.v[2]]);
        CHECK(a >= 0.f);
        total += a;
    }
    float expected = std::abs(geom::signedArea2(outer)) * 0.5f - std::abs(geom::signedArea2(hole)) * 0.5f;
    CHECK(total == doctest::Approx(expected).epsilon(0.01));

    CHECK_FALSE(anyTriangleCrossesBoundary(merged, tris));
}

TEST_CASE("bridgeHoles: two holes both get stitched in") {
    std::vector<Vec2> outer = {{0, 0}, {20, 0}, {20, 10}, {0, 10}};
    std::vector<Vec2> holeA = {{4, 6}, {4, 4}, {2, 4}, {2, 6}};
    std::vector<Vec2> holeB = {{16, 6}, {16, 4}, {14, 4}, {14, 6}};

    auto merged = bridgeHoles(outer, {holeA, holeB});

    auto present = [&](Vec2 p) {
        for (const auto& m : merged) {
            if (m == p) return true;
        }
        return false;
    };
    for (const auto& v : holeA) CHECK(present(v));
    for (const auto& v : holeB) CHECK(present(v));

    auto tris = triangulateSimplePolygon(merged);
    REQUIRE_FALSE(tris.empty());

    float total = 0.f;
    for (const auto& t : tris) {
        total += triangleArea(merged[t.v[0]], merged[t.v[1]], merged[t.v[2]]);
    }
    float expected = std::abs(geom::signedArea2(outer)) * 0.5f - std::abs(geom::signedArea2(holeA)) * 0.5f -
                      std::abs(geom::signedArea2(holeB)) * 0.5f;
    CHECK(total == doctest::Approx(expected).epsilon(0.01));
    CHECK_FALSE(anyTriangleCrossesBoundary(merged, tris));
}

TEST_CASE("offsetLevel: positive radius shrinks the outer boundary") {
    Level level;
    level.outer = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};

    auto original = std::abs(geom::signedArea2(level.outer));

    auto insetPolys = offsetLevel(level, 10.f);
    REQUIRE(insetPolys.size() == 1);
    auto insetArea = std::abs(geom::signedArea2(insetPolys[0].outer));

    CHECK(insetArea < original);
    CHECK(insetArea == doctest::Approx(2.f * (100.f - 20.f) * (100.f - 20.f)).epsilon(0.05));
}

TEST_CASE("offsetLevel: a hole grows outward and eats into the walkable area") {
    Level level;
    level.outer = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};
    level.holes = {{{40, 40}, {40, 60}, {60, 60}, {60, 40}}};

    auto insetPolys = offsetLevel(level, 5.f);
    REQUIRE(insetPolys.size() == 1);
    REQUIRE(insetPolys[0].holes.size() == 1);

    auto originalHoleArea = std::abs(geom::signedArea2(level.holes[0]));
    auto insetHoleArea = std::abs(geom::signedArea2(insetPolys[0].holes[0]));
    CHECK(insetHoleArea > originalHoleArea);
}

TEST_CASE("offsetLevel: radius larger than the region empties the walkable area") {
    Level level;
    level.outer = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};

    auto insetPolys = offsetLevel(level, 20.f);
    CHECK(insetPolys.empty());
}
