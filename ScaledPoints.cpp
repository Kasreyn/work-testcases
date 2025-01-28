#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cassert>

struct Point {
    float m_Y; // Vertical position
};

void testGridSpacing() {
    // Test parameters
    const int m_SizeX = 10;              // Number of columns per row
    const int m_BlockHeight = 32;        // Height of each row
    const int numPoints = 30;            // Total points to test (3 rows worth of points)

    // Expected results for m_Y
    const std::vector<int> expected_m_Y = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     // First row (indices 0–9)
        32, 32, 32, 32, 32, 32, 32, 32, 32, 32, // Second row (indices 10–19)
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64  // Third row (indices 20–29)
    };

    // Create a grid of points
    std::vector<Point> m_ScaledPoints(numPoints);
	m_ScaledPoints.resize(numPoints);

    // Populate the grid
    for (int index = 0; index < numPoints; ++index) {
        m_ScaledPoints[index].m_Y = (float)m_BlockHeight * (float)(index / m_SizeX);
		std::cout.precision(7);
		printf( "%6.*lf", 400, m_ScaledPoints[index].m_Y);
//		printf( "%f", m_ScaledPoints[index].m_Y);
		std::cout << " " << index << ": " << m_ScaledPoints[index].m_Y << std::endl;
//		std::cout << " : " << std::fixed << std::setprecision(6) << m_ScaledPoints[index].m_Y << std::endl;
//		std::cout << std::setprecision(10) << m_ScaledPoints[index].m_Y << std::endl;
    }

    // Validate results
    for (int index = 0; index < numPoints; ++index) {
        assert(static_cast<int>(m_ScaledPoints[index].m_Y) == expected_m_Y[index]);
    }

    // If all assertions pass
    std::cout << "Test passed! Grid spacing is correct.\n";
}

int main() {
    testGridSpacing();
    return 0;
}

