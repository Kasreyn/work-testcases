#include <algorithm>
#include <cstddef>
#include <iostream>
#include <chrono>
#include <vector>
#include <execution>
#include <random>
#include <immintrin.h>

// Assume this struct and function are defined elsewhere
struct Point {
    double m_X;
    double m_Y;
};

void BarycentricInterpolation(Point& point,
							  const Point& srcA,
							  const Point& srcB,
							  const Point& srcC,
							  const Point& dstA,
							  const Point& dstB,
							  const Point& dstC) {
	double denominator =
		(srcB.m_Y - srcC.m_Y) * (srcA.m_X - srcC.m_X) + (srcC.m_X - srcB.m_X) * (srcA.m_Y - srcC.m_Y);

	double l1 = ((srcB.m_Y - srcC.m_Y) * (point.m_X - srcC.m_X) +
				 (srcC.m_X - srcB.m_X) * (point.m_Y - srcC.m_Y)) /
				denominator;
	double l2 = ((srcC.m_Y - srcA.m_Y) * (point.m_X - srcC.m_X) +
				 (srcA.m_X - srcC.m_X) * (point.m_Y - srcC.m_Y)) /
				denominator;
	double l3 = 1.0 - l1 - l2;

	point.m_X = l1 * dstA.m_X + l2 * dstB.m_X + l3 * dstC.m_X;
	point.m_Y = l1 * dstA.m_Y + l2 * dstB.m_Y + l3 * dstC.m_Y;
}

int main() {
    const int iterations = 1'000'000'000;

    // Define triangle in source and destination
    Point srcA{0.0, 0.0}, srcB{1.0, 0.0}, srcC{0.0, 1.0};
    Point dstA{10.0, 10.0}, dstB{20.0, 10.0}, dstC{10.0, 20.0};

    // Prepare a vector of test points within the triangle
    std::vector<Point> testPoints;
    testPoints.reserve(iterations);

    std::mt19937 rng(42);  // fixed seed for reproducibility
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < iterations; ++i) {
        double u = dist(rng);
        double v = dist(rng);
        if (u + v > 1.0) {
            u = 1.0 - u;
            v = 1.0 - v;
        }
        double x = (1 - u - v) * srcA.m_X + u * srcB.m_X + v * srcC.m_X;
        double y = (1 - u - v) * srcA.m_Y + u * srcB.m_Y + v * srcC.m_Y;
        testPoints.push_back({x, y});
    }

	auto baryLamb = [&](Point& pt) {
		BarycentricInterpolation(pt, srcA, srcB, srcC, dstA, dstB, dstC);
	};

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    for (auto& p : testPoints) {
		baryLamb(p);
//        BarycentricInterpolation(p, srcA, srcB, srcC, dstA, dstB, dstC);
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> duration = end - start;

    std::cout << "Interpolated " << iterations << " points in "
              << duration.count() << " seconds.\n";


    // Start timing
    start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
	for (std::size_t i = 0; i < testPoints.size(); ++i) {
		baryLamb(testPoints[i]);
	}
//	std::for_each(std::execution::par, testPoints.begin(), testPoints.end(), baryLamb);

    end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    duration = end - start;

    std::cout << "std::execution::par Interpolated " << iterations << " points in "
              << duration.count() << " seconds.\n";

    return 0;
}

