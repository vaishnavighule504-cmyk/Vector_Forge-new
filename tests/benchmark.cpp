#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "CircleShape.h"
#include "Document.h"
#include "RectShape.h"

int main() {
    using Clock = std::chrono::steady_clock;
    using Micro = std::chrono::duration<double, std::micro>;

    const int sizes[] = {1000, 10000, 50000};
    const int kQueries = 2000;

    std::cout << std::left << std::setw(8) << "N"
              << std::setw(22) << "linear (us/query)"
              << std::setw(22) << "quadtree (us/query)"
              << std::setw(10) << "speedup"
              << std::setw(14) << "build (ms)"
              << "mismatches\n";

    for (int n : sizes) {
        std::mt19937 rng(42);   // fixed seed: same data on every run
        std::uniform_real_distribution<double> pos(0, 10000), size(5, 40);

        Document doc;
        for (int i = 0; i < n; ++i) {
            double x = pos(rng), y = pos(rng);
            if (i % 2 == 0) doc.addShape(std::make_unique<CircleShape>(QPointF(x, y), size(rng) / 2));
            else            doc.addShape(std::make_unique<RectShape>(QRectF(x, y, size(rng), size(rng))));
        }

        std::vector<QPointF> points;
        for (int i = 0; i < kQueries; ++i) points.emplace_back(pos(rng), pos(rng));

        // Build time (first query triggers the index build)
        auto b0 = Clock::now();
        doc.shapeAt(points[0]);
        double buildMs = Micro(Clock::now() - b0).count() / 1000.0;

        std::vector<Shape*> linearResults, treeResults;
        linearResults.reserve(kQueries);
        treeResults.reserve(kQueries);

        auto t0 = Clock::now();
        for (const auto& p : points) linearResults.push_back(doc.shapeAtLinear(p));
        double linearUs = Micro(Clock::now() - t0).count() / kQueries;

        auto t1 = Clock::now();
        for (const auto& p : points) treeResults.push_back(doc.shapeAt(p));
        double treeUs = Micro(Clock::now() - t1).count() / kQueries;

        int mismatches = 0;
        for (int i = 0; i < kQueries; ++i)
            if (linearResults[i] != treeResults[i]) ++mismatches;

        std::cout << std::left << std::fixed << std::setprecision(2)
                  << std::setw(8) << n
                  << std::setw(22) << linearUs
                  << std::setw(22) << treeUs
                  << std::setw(10) << (linearUs / treeUs)
                  << std::setw(14) << buildMs
                  << mismatches << "\n";
    }
}