#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>  // Include for output formatting
#include <sys/resource.h>

// Define Point struct with double precision
struct Point {
    double m_X;
    double m_Y;
};

// Define TableData struct with a large fixed-size array
struct TableData {
    static constexpr uint16_t MAX_WIDTH = 25500;
    static constexpr uint16_t MAX_HEIGHT = 128;
    Point m_Points[MAX_WIDTH * MAX_HEIGHT];
};

int main() {
	TableData t;

	while (true) {
	}

    return 0;
}

