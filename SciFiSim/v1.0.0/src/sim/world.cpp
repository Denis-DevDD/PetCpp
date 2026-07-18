#include "sim/world.hpp"

namespace sim {

World::World(int width, int height)
    : width_(width), height_(height), tiles_(static_cast<size_t>(width) * height) {
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

void World::step() {
    ++tick_;
}

bool World::placeBuilding(Vec2i p){
    if (!ifInBounds(p)) 
        return false;
    Tile& t = at(p);
    if (t.occupied) 
        return false;
    t.occupied = true;
    return true;
}

bool World::removeBuilding(Vec2i p) {
    if (!ifInBounds(p))
        return false;
    Tile& t = at(p);
    if (!t.occupied)
        return false;
    t.occupied = false;
    return true;
}

}