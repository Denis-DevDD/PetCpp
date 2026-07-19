#include <doctest/doctest.h>
#include <nav/builder.hpp>
#include <nav/geometry.hpp>

#include <cmath>

using namespace nav;

namespace {

float triangleArea(const NavMesh& mesh, TriId t) {
    const auto& tri = mesh.triangles()[t];
    const auto& vs = mesh.vertices();
    return std::abs(geom::orientation(vs[tri.v[0]], vs[tri.v[1]], vs[tri.v[2]])) * 0.5f;
}

bool adjacencyIsConsistent(const NavMesh& mesh) {
    const auto& tris = mesh.triangles();
    for (size_t i = 0; i < tris.size(); ++i) {
        for (int e = 0; e < 3; ++e) {
            TriId n = tris[i].neighbor[e];
            if (n == NoTri) continue;
            if (n < 0 || static_cast<size_t>(n) >= tris.size()) return false;
            const auto& back = tris[static_cast<size_t>(n)];
            bool linksBack = back.neighbor[0] == static_cast<TriId>(i) ||
                              back.neighbor[1] == static_cast<TriId>(i) ||
                              back.neighbor[2] == static_cast<TriId>(i);
            if (!linksBack) return false;
        }
    }
    return true;
}

}

TEST_CASE("build: simple rectangle produces two adjacent triangles") {
    Level level;
    level.outer = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());
    CHECK(mesh->triangles().size() == 2);
    CHECK(adjacencyIsConsistent(*mesh));

    int sharedEdges = 0, boundaryEdges = 0;
    for (const auto& tri : mesh->triangles()) {
        for (int e = 0; e < 3; ++e) {
            if (tri.neighbor[e] == NoTri) ++boundaryEdges;
            else ++sharedEdges;
        }
    }
    CHECK(sharedEdges == 2);
    CHECK(boundaryEdges == 4);
}

TEST_CASE("build: findTriangle locates a point via each triangle's centroid") {
    Level level;
    level.outer = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    for (TriId t = 0; t < static_cast<TriId>(mesh->triangles().size()); ++t) {
        Vec2 c = mesh->centroid(t);
        CHECK(mesh->findTriangle(c) == t);
    }
    CHECK(mesh->findTriangle({-100, -100}) == NoTri);
}

TEST_CASE("build: portal returns a shared edge between adjacent triangles") {
    Level level;
    level.outer = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());

    const auto& tri0 = mesh->triangles()[0];
    for (int e = 0; e < 3; ++e) {
        if (tri0.neighbor[e] == NoTri) continue;
        Vec2 left, right;
        mesh->portal(0, tri0.neighbor[e], left, right);
        const auto& vs = mesh->vertices();
        bool leftIsCorner = left == vs[tri0.v[0]] || left == vs[tri0.v[1]] || left == vs[tri0.v[2]];
        bool rightIsCorner = right == vs[tri0.v[0]] || right == vs[tri0.v[1]] || right == vs[tri0.v[2]];
        CHECK(leftIsCorner);
        CHECK(rightIsCorner);
    }
}

TEST_CASE("build: level with a hole covers outer-minus-hole area with consistent adjacency") {
    Level level;
    level.outer = {{0, 0}, {20, 0}, {20, 20}, {0, 20}};
    level.holes = {{{8, 8}, {8, 12}, {12, 12}, {12, 8}}};

    auto mesh = build(level);
    REQUIRE(mesh.has_value());
    CHECK(adjacencyIsConsistent(*mesh));

    float total = 0.f;
    for (TriId t = 0; t < static_cast<TriId>(mesh->triangles().size()); ++t) {
        total += triangleArea(*mesh, t);
    }
    float expected = std::abs(geom::signedArea2(level.outer)) * 0.5f -
        std::abs(geom::signedArea2(level.holes[0])) * 0.5f;
    CHECK(total == doctest::Approx(expected).epsilon(0.01));
}

TEST_CASE("build: radius that swallows the whole level yields no mesh") {
    Level level;
    level.outer = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};

    auto mesh = build(level, BuildSettings{20.f});
    CHECK_FALSE(mesh.has_value());
}

#include <nav/level.hpp>
#include <nav/query.hpp>

TEST_CASE("build: pillars.json (6 holes) covers full area and stays connected") {
    auto lvl = loadLevel("data/levels/pillars.json");
    REQUIRE(lvl.has_value());
    REQUIRE(lvl->holes.size() == 6);

    auto mesh = build(*lvl);
    REQUIRE(mesh.has_value());
    CHECK(adjacencyIsConsistent(*mesh));

    double area = 0.0;
    for (TriId t = 0; t < static_cast<TriId>(mesh->triangles().size()); ++t) {
        area += triangleArea(*mesh, t);
    }
    double expected = 800.0 * 600.0 - 6.0 * 70.0 * 70.0;
    CHECK(area == doctest::Approx(expected).epsilon(0.001));

    NavQuery q(*mesh);
    Path path = q.findPath({300, 300}, {500, 500});
    REQUIRE(path.size() >= 2);
    for (const auto& hole : lvl->holes) {
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            const size_t hn = hole.size();
            for (size_t k = 0; k < hn; ++k) {
                Vec2 ea = hole[k], eb = hole[(k + 1) % hn];
                if (path[i] == ea || path[i] == eb || path[i + 1] == ea || path[i + 1] == eb) continue;
                CHECK_FALSE(geom::segmentsIntersect(path[i], path[i + 1], ea, eb));
            }
        }
    }
}

TEST_CASE("build: stress.json (80 holes) covers full area and stays connected") {
    auto lvl = loadLevel("data/levels/stress.json");
    REQUIRE(lvl.has_value());
    REQUIRE(lvl->holes.size() == 80);

    auto mesh = build(*lvl);
    REQUIRE(mesh.has_value());
    CHECK(adjacencyIsConsistent(*mesh));

    double area = 0.0;
    for (TriId t = 0; t < static_cast<TriId>(mesh->triangles().size()); ++t) {
        area += triangleArea(*mesh, t);
    }
    double outerArea = std::abs(geom::signedArea2(lvl->outer)) * 0.5;
    double holeArea = 0.0;
    for (const auto& h : lvl->holes) holeArea += std::abs(geom::signedArea2(h)) * 0.5;
    CHECK(area == doctest::Approx(outerArea - holeArea).epsilon(0.001));

    NavQuery q(*mesh);
    CHECK(q.findPath({40, 40}, {1560, 1160}).size() >= 2);
}
