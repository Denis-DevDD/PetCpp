#pragma once

#include <nav/types.hpp>
#include <vector>

namespace nav::geom {

constexpr float kEpsilon = 1e-4f;

float signedArea2(const std::vector<Vec2>& poly);

float orientation(Vec2 a, Vec2 b, Vec2 c);

bool pointInTriangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c);

bool segmentsIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d);

}
