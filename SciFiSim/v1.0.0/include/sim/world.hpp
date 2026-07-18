#pragma once
#include "types.hpp"
#include <vector>

namespace sim {

    struct Tile {
        bool occupied = false;
    };

class World {
public:
    World(int width, int height);

    int getWidth()  const { return width_; }
    int getHeight() const { return height_; }
    Tick getTick()  const { return tick_; }

    bool ifInBounds(Vec2i p) const;

    const Tile& at(Vec2i p) const;
    Tile& at(Vec2i p);

    void step();

    bool placeBuilding(Vec2i p);
    bool removeBuilding(Vec2i p);

private:
    int width_;
    int height_;
    Tick tick_ = 0;
    std::vector<Tile> tiles_;
};

}