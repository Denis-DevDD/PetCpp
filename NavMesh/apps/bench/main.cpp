#include <nav/builder.hpp>
#include <nav/level.hpp>
#include <nav/query.hpp>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <random>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

double millisSince(Clock::time_point t0) {
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

nav::Vec2 randomMeshPoint(const nav::NavMesh& mesh, std::mt19937& rng) {
    const auto& vs = mesh.vertices();
    float minX = vs[0].x, maxX = vs[0].x, minY = vs[0].y, maxY = vs[0].y;
    for (const auto& v : vs) {
        minX = std::min(minX, v.x);
        maxX = std::max(maxX, v.x);
        minY = std::min(minY, v.y);
        maxY = std::max(maxY, v.y);
    }
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);

    for (int attempt = 0; attempt < 10000; ++attempt) {
        nav::Vec2 p{distX(rng), distY(rng)};
        if (mesh.findTriangle(p) != nav::NoTri) return p;
    }
    return vs[0];
}

}

int main(int argc, char** argv) {
    const char* levelPath = argc > 1 ? argv[1] : "data/levels/rooms.json";
    float agentRadius = argc > 2 ? std::stof(argv[2]) : 0.f;
    int numQueries = argc > 3 ? std::stoi(argv[3]) : 1000;

    auto level = nav::loadLevel(levelPath);
    if (!level) {
        std::fprintf(stderr, "Failed to load level: %s\n", levelPath);
        return 1;
    }

    auto t0 = Clock::now();
    auto mesh = nav::build(*level, nav::BuildSettings{agentRadius});
    double buildMs = millisSince(t0);

    if (!mesh) {
        std::fprintf(stderr, "No walkable area at radius %.1f\n", agentRadius);
        return 1;
    }

    nav::NavQuery query(*mesh);
    std::mt19937 rng(1234);

    std::vector<double> micros(static_cast<size_t>(numQueries));
    std::vector<size_t> pathLens(static_cast<size_t>(numQueries));
    std::vector<size_t> corridorLens(static_cast<size_t>(numQueries));

    for (int i = 0; i < numQueries; ++i) {
        nav::Vec2 start = randomMeshPoint(*mesh, rng);
        nav::Vec2 goal = randomMeshPoint(*mesh, rng);

        auto qt0 = Clock::now();
        nav::Path path = query.findPath(start, goal);
        double queryMicros = std::chrono::duration<double, std::micro>(Clock::now() - qt0).count();

        micros[static_cast<size_t>(i)] = queryMicros;
        pathLens[static_cast<size_t>(i)] = path.size();
        corridorLens[static_cast<size_t>(i)] = query.lastCorridor().size();
    }

    std::filesystem::create_directories("output");
    std::ofstream csv("output/bench.csv");
    csv << "query_index,micros,path_len,corridor_len\n";
    for (int i = 0; i < numQueries; ++i) {
        csv << i << ',' << micros[static_cast<size_t>(i)] << ',' << pathLens[static_cast<size_t>(i)] << ','
            << corridorLens[static_cast<size_t>(i)] << '\n';
    }

    std::vector<double> sorted = micros;
    std::sort(sorted.begin(), sorted.end());
    double mean = std::accumulate(sorted.begin(), sorted.end(), 0.0) / static_cast<double>(sorted.size());
    double median = sorted[sorted.size() / 2];

    std::printf("level: %s (agent radius %.1f)\n", levelPath, agentRadius);
    std::printf("build time: %.3f ms (%zu triangles)\n", buildMs, mesh->triangles().size());
    std::printf("queries: %d, mean: %.2f us, median: %.2f us\n", numQueries, mean, median);
    std::printf("wrote output/bench.csv\n");
    return 0;
}
