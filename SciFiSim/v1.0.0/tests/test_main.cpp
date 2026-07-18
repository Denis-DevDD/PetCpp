#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "sim/world.hpp"

TEST_CASE("world bounds") {
    sim::World w(10, 8);
    CHECK(w.width() == 10);
    CHECK(w.height() == 8);
    CHECK(w.inBounds({0, 0}));
    CHECK(w.inBounds({9, 7}));
    CHECK_FALSE(w.inBounds({10, 0}));
    CHECK_FALSE(w.inBounds({0, 8}));
    CHECK_FALSE(w.inBounds({-1, 0}));
}

TEST_CASE("tick advances") {
    sim::World w(4, 4);
    CHECK(w.tick() == 0);
    w.step();
    w.step();
    CHECK(w.tick() == 2);
}

TEST_CASE("tile occupancy") {
    sim::World w(4, 4);
    CHECK_FALSE(w.at({1, 1}).occupied);
    w.at({1, 1}).occupied = true;
    CHECK(w.at({1, 1}).occupied);
}