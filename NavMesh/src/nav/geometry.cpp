#include "geometry.hpp"

#include <algorithm>

namespace nav::geom {

float signedArea2(const std::vector<Vec2>& poly) {
    float sum = 0.f;
    const size_t n = poly.size();
    for (size_t i = 0; i < n; ++i) {
        const Vec2& p = poly[i];
        const Vec2& q = poly[(i + 1) % n];
        sum += p.x * q.y - q.x * p.y;
    }
    return sum;
}

float orientation(Vec2 a, Vec2 b, Vec2 c) {
    return cross(b - a, c - a);
}

bool pointInTriangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c) {

    float d1 = orientation(a, b, p);
    float d2 = orientation(b, c, p);
    float d3 = orientation(c, a, p);

    bool hasNeg = (d1 < -kEpsilon) || (d2 < -kEpsilon) || (d3 < -kEpsilon);
    bool hasPos = (d1 > kEpsilon) || (d2 > kEpsilon) || (d3 > kEpsilon);

    return !(hasNeg && hasPos);
}

namespace {

bool onSegment(Vec2 p, Vec2 q, Vec2 r) {

    return r.x >= std::min(p.x, q.x) - kEpsilon && r.x <= std::max(p.x, q.x) + kEpsilon &&
        r.y >= std::min(p.y, q.y) - kEpsilon && r.y <= std::max(p.y, q.y) + kEpsilon;
}

int sign(float v) {
    if (v > kEpsilon) return 1;
    if (v < -kEpsilon) return -1;
    return 0;
}

}

bool segmentsIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
    int d1 = sign(orientation(c, d, a));
    int d2 = sign(orientation(c, d, b));
    int d3 = sign(orientation(a, b, c));
    int d4 = sign(orientation(a, b, d));

    if (d1 != 0 && d2 != 0 && d1 != d2 && d3 != 0 && d4 != 0 && d3 != d4) return true;

    if (d1 == 0 && onSegment(c, d, a)) return true;
    if (d2 == 0 && onSegment(c, d, b)) return true;
    if (d3 == 0 && onSegment(a, b, c)) return true;
    if (d4 == 0 && onSegment(a, b, d)) return true;

    return false;
}

}
