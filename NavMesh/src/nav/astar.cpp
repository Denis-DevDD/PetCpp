#include "astar.hpp"

#include <algorithm>
#include <queue>
#include <unordered_map>

namespace nav {

namespace {

Vec2 portalMidpoint(const NavMesh& mesh, TriId a, TriId b) {
    Vec2 left, right;
    mesh.portal(a, b, left, right);
    return (left + right) * 0.5f;
}

struct Entry {
    float fScore;
    float gAtPush;
    TriId tri;
};

struct Compare {
    bool operator()(const Entry& a, const Entry& b) const { return a.fScore > b.fScore; }
};

}

std::vector<TriId> findCorridor(const NavMesh& mesh, Vec2 start, Vec2 goal, std::vector<TriId>* visitedOut) {
    if (visitedOut) visitedOut->clear();

    const TriId startTri = mesh.findTriangle(start);
    const TriId goalTri = mesh.findTriangle(goal);
    if (startTri == NoTri || goalTri == NoTri) return {};
    if (startTri == goalTri) return {startTri};

    std::unordered_map<TriId, float> gScore;
    std::unordered_map<TriId, TriId> cameFrom;
    std::unordered_map<TriId, Vec2> entryMid;
    std::priority_queue<Entry, std::vector<Entry>, Compare> open;

    gScore[startTri] = 0.f;
    entryMid[startTri] = start;
    open.push({length(start - goal), 0.f, startTri});

    while (!open.empty()) {
        Entry current = open.top();
        open.pop();

        if (current.gAtPush > gScore.at(current.tri) + 1e-4f) continue;

        if (visitedOut) visitedOut->push_back(current.tri);

        if (current.tri == goalTri) {
            std::vector<TriId> corridor{goalTri};
            TriId t = goalTri;
            while (t != startTri) {
                t = cameFrom.at(t);
                corridor.push_back(t);
            }
            std::reverse(corridor.begin(), corridor.end());
            return corridor;
        }

        const float currentG = gScore.at(current.tri);
        const Vec2 currentMid = entryMid.at(current.tri);

        for (int e = 0; e < 3; ++e) {
            TriId neighbor = mesh.triangles()[static_cast<size_t>(current.tri)].neighbor[e];
            if (neighbor == NoTri) continue;

            Vec2 mid = portalMidpoint(mesh, current.tri, neighbor);
            float tentativeG = currentG + length(currentMid - mid);
            auto it = gScore.find(neighbor);
            if (it == gScore.end() || tentativeG < it->second) {
                gScore[neighbor] = tentativeG;
                cameFrom[neighbor] = current.tri;
                entryMid[neighbor] = mid;
                float h = length(mid - goal);
                open.push({tentativeG + h, tentativeG, neighbor});
            }
        }
    }

    return {};
}

}
