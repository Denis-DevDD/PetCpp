#include "sim/contentloader.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <unordered_map>

using json = nlohmann::json;

namespace sim {

static RGB parseColor(const json& j) {

    RGB c;
    if (j.is_array() && j.size() == 3) {
        c.r = static_cast<unsigned char>(j[0].get<int>());
        c.g = static_cast<unsigned char>(j[1].get<int>());
        c.b = static_cast<unsigned char>(j[2].get<int>());
    }
    return c;
}

LoadResult loadContent(const std::string& path) {
    LoadResult res;

    std::ifstream file(path);
    if (!file) {
        res.error = "cannot open file: " + path;
        return res;
    }

    json root;
    try {
        file >> root;
    } catch (const std::exception& e) {
        res.error = std::string("json parse error: ") + e.what();
        return res;
    }

    try {
        std::unordered_map<std::string, ResourceId> resByName;
        for (const auto& jr : root.at("resources")) {
            ResourceDef def;
            def.name  = jr.at("name").get<std::string>();
            def.color = parseColor(jr.value("color", json::array()));
            ResourceId id = res.db.addResource(def);
            resByName[jr.at("id").get<std::string>()] = id;
        }

        auto resolveRes = [&](const std::string& name) -> ResourceId {
            auto it = resByName.find(name);
            if (it == resByName.end())
                throw std::runtime_error("unknown resource id: " + name);
            return it->second;
        };

        for (const auto& jb : root.at("buildings")) {
            BuildingDef def;
            def.name  = jb.at("name").get<std::string>();
            def.color = parseColor(jb.value("color", json::array()));

            if (jb.contains("recipe")) {
                const auto& jrec = jb.at("recipe");
                def.recipe.out    = resolveRes(jrec.at("out").get<std::string>());
                def.recipe.amount = jrec.at("amount").get<Amount>();
                def.recipe.period = jrec.at("period").get<Tick>();
                if (jrec.contains("inputs")) {
                    for (const auto& jin : jrec.at("inputs")) {
                        sim::RecipeInput in;
                        in.res    = resolveRes(jin.at("res").get<std::string>());
                        in.amount = jin.at("amount").get<Amount>();
                        def.recipe.inputs.push_back(in);
                    }
                }
            }
            res.db.add(def);
        }

        const auto& jp = root.at("population");
        res.popCfg.growthThreshold = jp.value("growthThreshold", 70);
        res.popCfg.declineThreshold = jp.value("declineThreshold", 30);
        res.popCfg.streakNeeded = jp.value("streakNeeded", Tick{3});
        res.popCfg.growthBatch = jp.value("growthBatch", Amount{2});

        std::unordered_map<std::string, int> tierIndex;
        const auto& jtiers = jp.at("tiers");
        for (const auto& jt : jtiers) {
            sim::TierDef tier;
            tier.name  = jt.at("name").get<std::string>();
            tier.color = parseColor(jt.value("color", json::array()));
            tier.multiplierMilli = jt.value("multiplier", 1000);
            for (const auto& jn : jt.at("needs")) {
                sim::TierNeed need;
                need.res       = resolveRes(jn.at("res").get<std::string>());
                need.perCapita = jn.value("amount", Amount{1});
                tier.needs.push_back(need);
            }
            tierIndex[jt.at("id").get<std::string>()] =
                static_cast<int>(res.popCfg.tiers.size());
            res.popCfg.tiers.push_back(tier);
        }

        if (root.contains("startStock")) {
            for (const auto& js : root.at("startStock")) {
                sim::RecipeInput s;
                s.res    = resolveRes(js.at("res").get<std::string>());
                s.amount = js.at("amount").get<Amount>();
                res.startStock.push_back(s);
            }
        }

        {
            size_t idx = 0;
            for (const auto& jt : jtiers) {
                if (jt.contains("upgrade")) {
                    const auto& ju = jt.at("upgrade");
                    sim::TierUpgrade up;

                    std::string toId = ju.at("to").get<std::string>();
                    auto it = tierIndex.find(toId);
                    if (it == tierIndex.end())
                        throw std::runtime_error("unknown tier id in upgrade: " + toId);
                    up.toTier = it->second;

                    up.satisfactionNeeded = ju.value("satisfactionNeeded", 80);
                    up.streakNeeded = ju.value("streakNeeded", Tick{5});
                    up.batch = ju.value("batch", Amount{2});

                    if (ju.contains("cost")) {
                        for (const auto& jc : ju.at("cost")) {
                            sim::RecipeInput c;
                            c.res = resolveRes(jc.at("res").get<std::string>());
                            c.amount = jc.at("amount").get<Amount>();
                            up.cost.push_back(c);
                        }
                    }
                    res.popCfg.tiers[idx].upgrade = up;
                }

                if (jt.contains("downgrade")) {
                    const auto& jd = jt.at("downgrade");
                    sim::TierDowngrade down;

                    std::string toId = jd.at("to").get<std::string>();
                    auto it = tierIndex.find(toId);
                    if (it == tierIndex.end())
                        throw std::runtime_error("unknown tier id in downgrade: " + toId);
                    down.toTier = it->second;

                    down.satisfactionBelow = jd.value("satisfactionBelow", 25);
                    down.streakNeeded = jd.value("streakNeeded", Tick{5});
                    down.batch = jd.value("batch", Amount{2});

                    res.popCfg.tiers[idx].downgrade = down;
                }

                ++idx;
            }
        }

    } catch (const std::exception& e) {
        res.error = std::string("content error: ") + e.what();
        return res;
    }

    res.ok = true;
    return res;
}

}