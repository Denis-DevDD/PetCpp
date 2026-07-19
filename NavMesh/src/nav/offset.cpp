#include <nav/offset.hpp>

#include <clipper2/clipper.h>

namespace nav {

namespace {

Clipper2Lib::PathD toClipperPath(const std::vector<Vec2>& poly) {
    Clipper2Lib::PathD path;
    path.reserve(poly.size());
    for (const auto& p : poly) path.emplace_back(p.x, p.y);
    return path;
}

std::vector<Vec2> fromClipperPath(const Clipper2Lib::PathD& path) {
    std::vector<Vec2> poly;
    poly.reserve(path.size());
    for (const auto& p : path) poly.push_back(Vec2{static_cast<float>(p.x), static_cast<float>(p.y)});
    return poly;
}

}

std::vector<OffsetPolygon> offsetLevel(const Level& level, float radius) {
    using namespace Clipper2Lib;

    if (radius <= 0.f) {
        return {OffsetPolygon{level.outer, level.holes}};
    }

    PathsD subject;
    subject.push_back(toClipperPath(level.outer));
    for (const auto& hole : level.holes) subject.push_back(toClipperPath(hole));

    PathsD inset = InflatePaths(subject, -static_cast<double>(radius), JoinType::Round, EndType::Polygon);

    std::vector<std::vector<Vec2>> outers;
    std::vector<std::vector<Vec2>> holes;
    for (const auto& path : inset) {
        if (path.size() < 3) continue;
        if (Area(path) > 0.0) {
            outers.push_back(fromClipperPath(path));
        } else {
            holes.push_back(fromClipperPath(path));
        }
    }

    std::vector<OffsetPolygon> result(outers.size());
    for (size_t i = 0; i < outers.size(); ++i) result[i].outer = outers[i];

    for (const auto& hole : holes) {
        if (hole.empty()) continue;
        const PointD probe(hole[0].x, hole[0].y);
        for (size_t i = 0; i < outers.size(); ++i) {
            const PathD outerPath = toClipperPath(outers[i]);
            if (PointInPolygon(probe, outerPath) == PointInPolygonResult::IsInside) {
                result[i].holes.push_back(hole);
                break;
            }
        }
    }

    return result;
}

}
