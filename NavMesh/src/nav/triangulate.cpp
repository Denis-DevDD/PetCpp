#include "triangulate.hpp"

#include "geometry.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

namespace nav {

namespace {

bool isConvexOrFlat(const std::vector<Vec2>& poly, int prev, int cur, int next) {
    return geom::orientation(poly[prev], poly[cur], poly[next]) >= -geom::kEpsilon;
}

bool isReflexAt(const std::vector<Vec2>& poly, int prev, int cur, int next) {
    return geom::orientation(poly[prev], poly[cur], poly[next]) < -geom::kEpsilon;
}

bool isEar(const std::vector<Vec2>& poly, const std::vector<int>& idx, size_t i) {
    const size_t m = idx.size();
    const int prev = idx[(i + m - 1) % m];
    const int cur = idx[i];
    const int next = idx[(i + 1) % m];

    if (!isConvexOrFlat(poly, prev, cur, next)) return false;

    for (size_t jp = 0; jp < m; ++jp) {
        const int j = idx[jp];
        if (j == prev || j == cur || j == next) continue;
        if (poly[j] == poly[prev] || poly[j] == poly[cur] || poly[j] == poly[next]) continue;
        const int jPrev = idx[(jp + m - 1) % m];
        const int jNext = idx[(jp + 1) % m];
        if (!isReflexAt(poly, jPrev, j, jNext)) continue;
        if (geom::pointInTriangle(poly[j], poly[prev], poly[cur], poly[next])) return false;
    }
    return true;
}

}

std::vector<RawTriangle> triangulateSimplePolygon(const std::vector<Vec2>& poly) {
    std::vector<RawTriangle> triangles;
    const size_t n = poly.size();
    if (n < 3) return triangles;
    triangles.reserve(n - 2);

    std::vector<int> idx(n);
    std::iota(idx.begin(), idx.end(), 0);

    size_t i = 0;
    size_t sinceLastClip = 0;
    while (idx.size() > 3) {
        const size_t m = idx.size();
        if (isEar(poly, idx, i)) {
            const int prev = idx[(i + m - 1) % m];
            const int cur = idx[i];
            const int next = idx[(i + 1) % m];
            triangles.push_back({{prev, cur, next}});
            idx.erase(idx.begin() + static_cast<ptrdiff_t>(i));
            if (i >= idx.size()) i = 0;
            sinceLastClip = 0;
        } else {
            i = (i + 1) % m;

            if (++sinceLastClip > m) return triangles;
        }
    }

    if (idx.size() == 3) {
        triangles.push_back({{idx[0], idx[1], idx[2]}});
    }
    return triangles;
}

namespace {

int rightmostVertex(const std::vector<Vec2>& hole) {
    int h = 0;
    for (int i = 1; i < static_cast<int>(hole.size()); ++i) {
        if (hole[i].x > hole[h].x) h = i;
    }
    return h;
}

struct BridgePoint {
    int existingVertex = -1;
    int insertAfter = -1;
    Vec2 point{};
};

BridgePoint findBridge(const std::vector<Vec2>& combined, Vec2 M) {
    const int n = static_cast<int>(combined.size());

    float bestX = std::numeric_limits<float>::max();
    int edgeA = -1;
    Vec2 I{};
    for (int i = 0; i < n; ++i) {
        Vec2 A = combined[i];
        Vec2 B = combined[(i + 1) % n];
        if (A.y == B.y) continue;
        if ((A.y > M.y && B.y > M.y) || (A.y < M.y && B.y < M.y)) continue;
        float t = (M.y - A.y) / (B.y - A.y);
        if (t < 0.f || t > 1.f) continue;
        float x = A.x + t * (B.x - A.x);
        if (x < M.x - geom::kEpsilon) continue;
        if (x < bestX) {
            bestX = x;
            I = Vec2{x, M.y};
            edgeA = i;
        }
    }

    BridgePoint bp;
    if (edgeA < 0) {

        int best = 0;
        float bd = lengthSq(combined[0] - M);
        for (int i = 1; i < n; ++i) {
            float d = lengthSq(combined[i] - M);
            if (d < bd) { bd = d; best = i; }
        }
        bp.existingVertex = best;
        return bp;
    }

    int edgeB = (edgeA + 1) % n;
    if (lengthSq(combined[edgeA] - I) <= geom::kEpsilon * geom::kEpsilon) {
        bp.existingVertex = edgeA;
    } else if (lengthSq(combined[edgeB] - I) <= geom::kEpsilon * geom::kEpsilon) {
        bp.existingVertex = edgeB;
    } else {
        bp.insertAfter = edgeA;
        bp.point = I;
    }
    return bp;
}

std::vector<Vec2> spliceHole(const std::vector<Vec2>& combined, const std::vector<Vec2>& hole,
                              int cIdx, int hIdx) {
    const int hn = static_cast<int>(hole.size());
    std::vector<Vec2> result;
    result.reserve(combined.size() + hole.size() + 2);

    for (int i = 0; i <= cIdx; ++i) result.push_back(combined[i]);
    for (int k = 0; k < hn; ++k) result.push_back(hole[(hIdx + k) % hn]);
    result.push_back(hole[hIdx]);
    result.push_back(combined[cIdx]);
    for (int i = cIdx + 1; i < static_cast<int>(combined.size()); ++i) result.push_back(combined[i]);

    return result;
}

}

std::vector<Vec2> bridgeHoles(const std::vector<Vec2>& outer, const std::vector<std::vector<Vec2>>& holes) {

    std::vector<int> order;
    for (int i = 0; i < static_cast<int>(holes.size()); ++i) {
        if (holes[i].size() >= 3) order.push_back(i);
    }
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return holes[a][rightmostVertex(holes[a])].x > holes[b][rightmostVertex(holes[b])].x;
    });

    std::vector<Vec2> combined = outer;
    for (int hi : order) {
        int hIdx = rightmostVertex(holes[hi]);
        BridgePoint bp = findBridge(combined, holes[hi][hIdx]);
        int cIdx;
        if (bp.existingVertex >= 0) {
            cIdx = bp.existingVertex;
        } else {

            combined.insert(combined.begin() + bp.insertAfter + 1, bp.point);
            cIdx = bp.insertAfter + 1;
        }
        combined = spliceHole(combined, holes[hi], cIdx, hIdx);
    }
    return combined;
}

}
