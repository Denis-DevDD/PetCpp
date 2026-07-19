#include "sim/world.hpp"
#include <fstream>
#include <cstdint>

namespace sim {

static constexpr std::uint32_t SAVE_MAGIC = 0x53465356; // 'S''F''S''V'
static constexpr std::uint32_t SAVE_VERSION = 1;

template <typename T>
static void writeRaw(std::ostream& os, const T& v) {
    os.write(reinterpret_cast<const char*>(&v), sizeof(T));
}

template <typename T>
static bool readRaw(std::istream& is, T& v) {
    is.read(reinterpret_cast<char*>(&v), sizeof(T));
    return static_cast<bool>(is);
}

bool saveWorld(const World& w, const std::string& path) {
    std::ofstream os(path, std::ios::binary);
    if (!os) return false;

    writeRaw(os, SAVE_MAGIC);
    writeRaw(os, SAVE_VERSION);

    writeRaw<std::int32_t>(os, w.width_);
    writeRaw<std::int32_t>(os, w.height_);
    writeRaw<std::int64_t>(os, w.tick_);

    writeRaw<std::uint32_t>(os, static_cast<std::uint32_t>(w.stock_.size()));
    for (Amount a : w.stock_) writeRaw<std::int64_t>(os, a);

    writeRaw<std::uint32_t>(os, static_cast<std::uint32_t>(w.tiles_.size()));
    for (const Tile& t : w.tiles_) {
        writeRaw<std::int32_t>(os, t.building);
        writeRaw<std::int64_t>(os, t.productionTick);
    }

    writeRaw<std::uint32_t>(os, static_cast<std::uint32_t>(w.tiers_.size()));
    for (const TierState& ts : w.tiers_) {
        writeRaw<std::int64_t>(os, ts.count);
        writeRaw<std::int32_t>(os, ts.satisfaction);
        writeRaw<std::int64_t>(os, ts.goodStreak);
        writeRaw<std::int64_t>(os, ts.badStreak);
        writeRaw<std::int64_t>(os, ts.upgradeStreak);
        writeRaw<std::int64_t>(os, ts.downgradeStreak);
    }

    return static_cast<bool>(os);
}

bool loadWorld(World& w, const std::string& path) {
    std::ifstream is(path, std::ios::binary);
    if (!is) return false;

    std::uint32_t magic = 0, version = 0;
    if (!readRaw(is, magic) || magic != SAVE_MAGIC) return false;
    if (!readRaw(is, version) || version != SAVE_VERSION) return false;

    std::int32_t width = 0, height = 0;
    std::int64_t tick = 0;
    if (!readRaw(is, width) || !readRaw(is, height) || !readRaw(is, tick))
        return false;

    if (width != w.width_ || height != w.height_) return false;
    w.tick_ = tick;

    std::uint32_t stockN = 0;
    if (!readRaw(is, stockN)) return false;
    if (stockN != w.stock_.size()) return false;   // контент должен совпадать
    for (auto& a : w.stock_) if (!readRaw(is, a)) return false;

    std::uint32_t tilesN = 0;
    if (!readRaw(is, tilesN)) return false;
    if (tilesN != w.tiles_.size()) return false;
    for (Tile& t : w.tiles_) {
        if (!readRaw(is, t.building)) return false;
        if (!readRaw(is, t.productionTick)) return false;
    }

    std::uint32_t tiersN = 0;
    if (!readRaw(is, tiersN)) return false;
    if (tiersN != w.tiers_.size()) return false;
    for (TierState& ts : w.tiers_) {
        if (!readRaw(is, ts.count)) return false;
        if (!readRaw(is, ts.satisfaction)) return false;
        if (!readRaw(is, ts.goodStreak)) return false;
        if (!readRaw(is, ts.badStreak)) return false;
        if (!readRaw(is, ts.upgradeStreak)) return false;
        if (!readRaw(is, ts.downgradeStreak)) return false;
    }

    return true;
}

}