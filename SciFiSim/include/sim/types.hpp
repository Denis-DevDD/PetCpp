#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace sim {

using Amount = std::int64_t;
using Tick = std::int64_t;

using BuildingId = int;
constexpr BuildingId NoBuilding = -1;

using ResourceId = int;
constexpr ResourceId NoResource = -1;

struct RGB {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
};

struct RecipeInput {
    ResourceId res = NoResource;
    Amount amount = 0;
};

struct Recipe {
    std::vector<RecipeInput> inputs;
    ResourceId out = NoResource;
    Amount amount = 0;
    Tick period = 1;

    bool valid() const { return out != NoResource && amount > 0 && period > 0; }
};

struct TierUpgrade {
    int toTier = -1;
    std::vector<RecipeInput> cost;
    int satisfactionNeeded = 80;
    Tick streakNeeded = 5;
    Amount batch = 2;
    bool valid() const { return toTier >= 0; }
};

struct TierNeed {
    ResourceId res = NoResource;
    Amount perCapita = 1;
};

struct TierDowngrade {
    int toTier = -1;
    int satisfactionBelow = 25;
    Tick streakNeeded = 5;
    Amount batch = 2;
    bool valid() const { return toTier >= 0; }
};

struct TierDef {
    std::string name;
    RGB color;
    std::vector<TierNeed> needs;
    int multiplierMilli = 1000;
    TierUpgrade upgrade;
    TierDowngrade downgrade;
};

struct PopConfig {

    std::vector<TierDef> tiers;

    int  growthThreshold  = 70;
    int  declineThreshold = 30;
    Tick streakNeeded     = 3;
    Amount growthBatch    = 2;
};

struct Vec2i {
    int x = 0;
    int y = 0;
    bool operator==(const Vec2i& o) const { return x == o.x && y == o.y; }
};

struct BuildingDef {
    std::string name;
    RGB color;

    Recipe recipe;
};

struct ResourceDef {
    std::string name;
    RGB color;
};

class ContentDB {

public:

    BuildingId add(BuildingDef def) {
        buildings_.push_back(std::move(def));
        return static_cast<BuildingId>(buildings_.size() - 1);
    }

    int count() const { return static_cast<int>(buildings_.size()); }

    bool valid(BuildingId id) const {
        return id >= 0 && id < count();
    }

    const BuildingDef& get(BuildingId id) const {
        return buildings_[static_cast<size_t>(id)];
    }

     ResourceId addResource(ResourceDef def) {
        resources_.push_back(std::move(def));
        return static_cast<ResourceId>(resources_.size() - 1);
    }
    int resourceCount() const { return static_cast<int>(resources_.size()); }
    bool validResource(ResourceId id) const {
        return id >= 0 && id < resourceCount();
    }
    const ResourceDef& getResource(ResourceId id) const {
        return resources_[static_cast<size_t>(id)];
    }

private:
    std::vector<BuildingDef> buildings_;
    std::vector<ResourceDef> resources_;
};

}