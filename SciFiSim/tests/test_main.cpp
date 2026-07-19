#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "sim/world.hpp"
#include "sim/contentloader.hpp"
#include <filesystem>

static sim::World makeWorld(sim::ContentDB& db, sim::PopConfig& cfg) {
    sim::LoadResult r = sim::loadContent("data/content.json");
    REQUIRE(r.ok);
    db = std::move(r.db);
    cfg = r.popCfg;
    return sim::World(20, 15, db.resourceCount(), cfg);
}

TEST_CASE("determinism: same run -> same hash") {
    sim::ContentDB db1, db2;
    sim::PopConfig c1, c2;
    sim::World a = makeWorld(db1, c1);
    sim::World b = makeWorld(db2, c2);

    for (sim::World* w : { &a, &b }) {
        w->placeBuilding({ 0, 0 }, 0);
        w->placeBuilding({ 1, 0 }, 1);
    }

    for (int i = 0; i < 1000; ++i) { a.step(db1); b.step(db2); }

    CHECK(a.hashState() == b.hashState());
}

TEST_CASE("production accumulates resources") {
    sim::ContentDB db;
    sim::PopConfig cfg;
    sim::World w = makeWorld(db, cfg);

    w.placeBuilding({ 0, 0 }, 0);
    sim::Amount before = w.stock(0);
    for (int i = 0; i < 10; ++i) w.step(db);
    CHECK(w.stock(0) > before);
}

TEST_CASE("save roundtrip preserves state") {
    sim::ContentDB db;
    sim::PopConfig cfg;
    sim::World w = makeWorld(db, cfg);

    w.placeBuilding({ 2, 2 }, 0);
    for (int i = 0; i < 50; ++i) w.step(db);
    std::uint64_t before = w.hashState();

    std::filesystem::create_directories("output");
    REQUIRE(sim::saveWorld(w, "output/test_save.bin"));

    sim::World w2(20, 15, db.resourceCount(), cfg);
    REQUIRE(sim::loadWorld(w2, "output/test_save.bin"));

    CHECK(w2.hashState() == before);
}

TEST_CASE("world bounds") {
    sim::ContentDB db;
    sim::PopConfig cfg;
    sim::World w = makeWorld(db, cfg);
    CHECK(w.ifInBounds({ 0, 0 }));
    CHECK(w.ifInBounds({ 19, 14 }));
    CHECK_FALSE(w.ifInBounds({ 20, 0 }));
    CHECK_FALSE(w.ifInBounds({ -1, 0 }));
}