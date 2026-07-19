#pragma once

#include <cmath>
#include <vector>

namespace nav {

struct Vec2 {
    float x = 0.f, y = 0.f;
};

inline Vec2 operator+(Vec2 a, Vec2 b) { return {a.x + b.x, a.y + b.y}; }
inline Vec2 operator-(Vec2 a, Vec2 b) { return {a.x - b.x, a.y - b.y}; }
inline Vec2 operator*(Vec2 a, float s) { return {a.x * s, a.y * s}; }
inline Vec2 operator*(float s, Vec2 a) { return {a.x * s, a.y * s}; }
inline bool operator==(Vec2 a, Vec2 b) { return a.x == b.x && a.y == b.y; }

inline float dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
// 2D "cross product": scalar z-component of the 3D cross of (a,0) x (b,0).
inline float cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
inline float length(Vec2 a) { return std::sqrt(dot(a, a)); }
inline float lengthSq(Vec2 a) { return dot(a, a); }
inline Vec2 normalized(Vec2 a) {
    float len = length(a);
    return len > 0.f ? Vec2{a.x / len, a.y / len} : Vec2{0.f, 0.f};
}

using TriId = int;
constexpr TriId NoTri = -1;

using Path = std::vector<Vec2>;

} // namespace nav
