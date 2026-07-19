#pragma once
#include "sim/types.hpp"
#include <vector>

namespace sim {

struct Tile {
    BuildingId building = NoBuilding;

    Tick productionTick = 0;
    bool occupied() const { return building != NoBuilding; }
};

struct TierState {
    Amount count = 0;
    int satisfaction = 100;
    Tick goodStreak = 0;
    Tick badStreak = 0;
    Tick upgradeStreak = 0;
    Tick downgradeStreak = 0;
};

class World {
public:
    World(int width, int height, int recourceCount, PopConfig popCfg);

    int getWidth()  const { return width_; }
    int getHeight() const { return height_; }
    Tick getTick()  const { return tick_; }

    void setPopConfig(PopConfig cfg) { popCfg_ = cfg; }

    bool ifInBounds(Vec2i p) const;

    const Tile& at(Vec2i p) const;
    Tile& at(Vec2i p);

    void step(const ContentDB& db);

    bool placeBuilding(Vec2i p, BuildingId id);
    bool removeBuilding(Vec2i p);

    Amount stock(ResourceId id) const;

    int getTierCount() const { return static_cast<int>(tiers_.size()); }
    const TierState& getTier(int i) const { return tiers_[static_cast<size_t>(i)]; }

    Amount getTotalPopulation() const;

    int getProductionMultiplierMilli() const;

private:

    void runProduction(const ContentDB& db);
    void runConsumption();
    void runPopulation();
    void runProgression();

    int width_;
    int height_;
    Tick tick_ = 0;
    std::vector<Tile> tiles_;
    std::vector<Amount> stock_;

    PopConfig popCfg_;
    std::vector<TierState> tiers_;
};

}