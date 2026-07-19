#include "sim/world.hpp"

namespace sim {

World::World(int width, int height, int resourceCount, PopConfig popCfg)
    : width_(width), height_(height),
      tiles_(static_cast<size_t>(width) * height),
      stock_(static_cast<size_t>(resourceCount), 0),
      popCfg_(popCfg) {
    tiers_.resize(popCfg_.tiers.size());
    if (!tiers_.empty()) {

        tiers_[0].count = 10;
        tiers_[0].satisfaction = 100;
    }
}

bool World::ifInBounds(Vec2i p) const {
    return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_;
}

const Tile& World::at(Vec2i p) const {
    return tiles_[static_cast<size_t>(p.y) * width_ + p.x];
}

Tile& World::at(Vec2i p) {
    return tiles_[static_cast<size_t>(p.y) * width_ + p.x];
}

Amount World::stock(ResourceId id) const {
    if (id < 0 || id >= static_cast<int>(stock_.size())) return 0;
    return stock_[static_cast<size_t>(id)];
}

bool World::placeBuilding(Vec2i p, BuildingId id){
    if (!ifInBounds(p)) 
        return false;
    Tile& t = at(p);
    if (t.occupied()) 
        return false;
    t.building = id;
    t.productionTick = 0;
    return true;
}

bool World::removeBuilding(Vec2i p) {
    if (!ifInBounds(p))
        return false;
    Tile& t = at(p);
    if (!t.occupied())
        return false;
    t.building = NoBuilding;
    t.productionTick = 0;
    return true;
}

void World::step(const ContentDB& db) {

    runProduction(db);
    runConsumption();
    runPopulation();
    runProgression();
    ++tick_;

}

void World::runProduction(const ContentDB& db) {
    for (Tile& t : tiles_) {
        if (!t.occupied()) continue;

        const BuildingDef& def = db.get(t.building);
        const Recipe& r = def.recipe;
        if (!r.valid()) continue;

        ++t.productionTick;
        if (t.productionTick < r.period) continue;   // ещё не время

        bool enough = true;
        for (const RecipeInput& in : r.inputs) {
            if (stock_[static_cast<size_t>(in.res)] < in.amount) {
                enough = false;
                break;
            }
        }

        if (!enough) {
            t.productionTick = r.period;
            continue;
        }

        for (const RecipeInput& in : r.inputs) {
            stock_[static_cast<size_t>(in.res)] -= in.amount;
        }

        int mult = getProductionMultiplierMilli();
        Amount produced = r.amount * mult / 1000;
        stock_[static_cast<size_t>(r.out)] += produced;

        t.productionTick = 0;
    }
}

Amount World::getTotalPopulation() const {
    Amount sum = 0;
    for (const TierState& t : tiers_) sum += t.count;
    return sum;
}

int World::getProductionMultiplierMilli() const {
    Amount total = 0;
    Amount weighted = 0;
    for (size_t i = 0; i < tiers_.size(); ++i) {
        Amount c = tiers_[i].count;
        if (c <= 0) continue;
        total += c;
        weighted += c * popCfg_.tiers[i].multiplierMilli;
    }
    if (total <= 0) 
        return 1000;
    return static_cast<int>(weighted / total);
}

void World::runConsumption() {

    for (size_t i = 0; i < tiers_.size(); ++i) {
        
        TierState& ts = tiers_[i];
        const TierDef& def = popCfg_.tiers[i];

        if (ts.count <= 0) 
            continue;

        bool fed = true;
        for (const TierNeed& need : def.needs) {
            Amount required = ts.count * need.perCapita;
            Amount have = stock_[static_cast<size_t>(need.res)];
            if (have < required) { 
                fed = false;
            }
        }

        for (const TierNeed& need : def.needs) {
            Amount required = ts.count * need.perCapita;
            Amount have = stock_[static_cast<size_t>(need.res)];
            Amount eaten = std::min(required, have);
            stock_[static_cast<size_t>(need.res)] -= eaten;
        }

        if (fed) 
            ts.satisfaction += 5;
        else
            ts.satisfaction -= 10;

        if (ts.satisfaction > 100)
            ts.satisfaction = 100;
        if (ts.satisfaction < 0)
            ts.satisfaction = 0;

    }
}

void World::runPopulation() {

    for (TierState& ts : tiers_) {
        if (ts.satisfaction >= popCfg_.growthThreshold) {
            ++ts.goodStreak;
            ts.badStreak = 0;
        } else if (ts.satisfaction <= popCfg_.declineThreshold) {
            ++ts.badStreak;
            ts.goodStreak = 0;
        } else {
            ts.goodStreak = 0;
            ts.badStreak = 0;
        }

        if (ts.goodStreak >= popCfg_.streakNeeded) {
            ts.count += popCfg_.growthBatch;
            ts.goodStreak = 0;
        }
        if (ts.badStreak >= popCfg_.streakNeeded) {
            ts.count -= popCfg_.growthBatch;
            if (ts.count < 0) ts.count = 0;
            ts.badStreak = 0;
        }
    }
}

void World::runProgression() {
    for (size_t i = 0; i < tiers_.size(); ++i) {
        
        TierState& ts = tiers_[i];
        const TierDef& tdef = popCfg_.tiers[i];
        const TierUpgrade& up = tdef.upgrade;
        const TierDowngrade& down = tdef.downgrade;

        if (up.valid() && ts.count > 0 && ts.satisfaction >= up.satisfactionNeeded) {
            ++ts.upgradeStreak;
        } else {
            ts.upgradeStreak = 0;
        }

        if (up.valid() && ts.upgradeStreak >= up.streakNeeded &&
            up.toTier >= 0 && up.toTier < static_cast<int>(tiers_.size())) {

            bool canPay = true;
            for (const RecipeInput& c : up.cost) {
                if (stock_[static_cast<size_t>(c.res)] < c.amount) { canPay = false; break; }
            }
            if (canPay) {
                Amount moving = std::min(up.batch, ts.count);
                if (moving > 0) {
                    for (const RecipeInput& c : up.cost)
                        stock_[static_cast<size_t>(c.res)] -= c.amount;
                    ts.count -= moving;
                    tiers_[static_cast<size_t>(up.toTier)].count += moving;
                    ts.upgradeStreak = 0;
                }
            }
        }

        if (down.valid() && ts.count > 0 && ts.satisfaction < down.satisfactionBelow) {
            ++ts.downgradeStreak;
        } else {
            ts.downgradeStreak = 0;
        }

        if (down.valid() && ts.downgradeStreak >= down.streakNeeded &&
            down.toTier >= 0 && down.toTier < static_cast<int>(tiers_.size())) {

            Amount moving = std::min(down.batch, ts.count);
            if (moving > 0) {
                ts.count -= moving;
                tiers_[static_cast<size_t>(down.toTier)].count += moving;
                ts.downgradeStreak = 0;
            }
        }
    }
}
}