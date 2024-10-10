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
    static constexpr uint16_t MAX_WIDTH = 255;
    static constexpr uint16_t MAX_HEIGHT = 128;

    uint16_t m_BlockWidth;
    uint16_t m_BlockHeight;
    uint16_t m_SizeX;
    uint16_t m_SizeY;
    uint16_t m_EntryCount;

    Point m_Points[MAX_WIDTH * MAX_HEIGHT];
};

constexpr size_t POINT_COUNT = TableData::MAX_WIDTH * TableData::MAX_HEIGHT;

// Benchmark for pure stack allocation
void stackAllocationBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();

    TableData tableData;
    for (size_t i = 0; i < POINT_COUNT; ++i) {
        tableData.m_Points[i].m_X = 1.23;
        tableData.m_Points[i].m_Y = 4.56;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Pure stack allocation took:                            "
              << std::fixed << std::setw(15) << std::setprecision(10) << duration.count() << " seconds\n";
}

// Benchmark for stack allocation with dynamic vector resizing
void stackWithDynamicVectorBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Point> points;
    points.resize(POINT_COUNT);
    for (size_t i = 0; i < POINT_COUNT; ++i) {
        points[i].m_X = 1.23;
        points[i].m_Y = 4.56;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Stack allocation with dynamic vector resizing took:    "
              << std::fixed << std::setw(15) << std::setprecision(10) << duration.count() << " seconds\n";
}

// Benchmark for dynamic allocation with unique_ptr
void dynamicAllocationBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();

    auto tableData = std::make_unique<TableData>();
    for (size_t i = 0; i < POINT_COUNT; ++i) {
        tableData->m_Points[i].m_X = 1.23;
        tableData->m_Points[i].m_Y = 4.56;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Dynamic allocation with unique_ptr took:               "
              << std::fixed << std::setw(15) << std::setprecision(10) << duration.count() << " seconds\n";
}

int main() {
	/*
	const struct rlimit rl {
		.rlim_cur = 1L * 1024L * 1024L
	};
*/
	struct rlimit rl;
	getrlimit(RLIMIT_STACK, &rl);
	rl.rlim_cur = 8L * 1024L * 1024L;
	int result = setrlimit(RLIMIT_STACK, &rl);
	if (result != 0) {
		fprintf(stderr, "setrlimit returned result = %d\n", result);
	}
    stackAllocationBenchmark();
    stackWithDynamicVectorBenchmark();
    dynamicAllocationBenchmark();
    return 0;
}

