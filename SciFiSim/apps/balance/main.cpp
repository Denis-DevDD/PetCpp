#include "sim/world.hpp"
#include "sim/contentloader.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <filesystem>

int main(int argc, char** argv) {
    int ticks  = (argc > 1) ? std::atoi(argv[1]) : 2000;
    int everyK = (argc > 2) ? std::atoi(argv[2]) : 1;
    std::string outPath = (argc > 3) ? argv[3] : "output/balance.csv";
    if (everyK < 1) 
        everyK = 1;

    std::filesystem::create_directories("output");
    sim::LoadResult loaded = sim::loadContent("data/content.json");
    if (!loaded.ok) {
        std::fprintf(stderr, "content load failed: %s\n", loaded.error.c_str());
        return 1;
    }
    sim::ContentDB db = std::move(loaded.db);
    sim::PopConfig cfg = loaded.popCfg;

    sim::World world(20, 15, db.resourceCount(), cfg, loaded.startStock);

    auto fill = [&](int bx, int by, int w, int h, sim::BuildingId id) {
        for (int y = by; y < by + h; ++y)
            for (int x = bx; x < bx + w; ++x)
                world.placeBuilding({ x, y }, id);
    };
    fill(0, 0, 3, 2, 0);
    fill(0, 3, 5, 3, 1);

    std::FILE* f = std::fopen(outPath.c_str(), "w");
    if (!f) {
        std::fprintf(stderr, "cannot open output: %s\n", outPath.c_str());
        return 1;
    }

    std::fprintf(f, "tick,mult");
    for (int i = 0; i < db.resourceCount(); ++i)
        std::fprintf(f, ",%s", db.getResource(i).name.c_str());
    for (int i = 0; i < world.getTierCount(); ++i)
        std::fprintf(f, ",%s", cfg.tiers[static_cast<size_t>(i)].name.c_str());
    std::fprintf(f, "\n");

    for (int t = 0; t <= ticks; ++t) {
        if (t % everyK == 0) {
            std::fprintf(f, "%lld,%d",
                         (long long)world.getTick(),
                         world.getProductionMultiplierMilli());
            for (int i = 0; i < db.resourceCount(); ++i)
                std::fprintf(f, ",%lld", (long long)world.stock(i));
            for (int i = 0; i < world.getTierCount(); ++i)
                std::fprintf(f, ",%lld", (long long)world.getTier(i).count);
            std::fprintf(f, "\n");
        }
        world.step(db);
    }

    std::fclose(f);
    std::printf("done: %d ticks -> %s (final hash: %llu)\n",
                ticks, outPath.c_str(),
                (unsigned long long)world.hashState());
    return 0;
}