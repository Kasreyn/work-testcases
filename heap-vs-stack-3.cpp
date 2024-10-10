#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>  // Include for output formatting
#include <sys/resource.h>

struct Point {
    float m_X;
    float m_Y;
};

struct FixedSizeTableData {
    static constexpr uint16_t MAX_WIDTH = 255;
    static constexpr uint16_t MAX_HEIGHT = 128;

    uint16_t m_BlockWidth;
    uint16_t m_BlockHeight;
    uint16_t m_SizeX;
    uint16_t m_SizeY;
    uint16_t m_EntryCount;

    Point m_Points[MAX_WIDTH * MAX_HEIGHT];
};

struct DynamicSizeTableData {
    uint16_t m_BlockWidth;
    uint16_t m_BlockHeight;
    uint16_t m_SizeX;
    uint16_t m_SizeY;
    uint16_t m_EntryCount;

    std::vector<Point> m_Points;  // Dynamic container
};

// Function that creates a DynamicSizeTableData object with a dynamic number of points
std::unique_ptr<DynamicSizeTableData> createDynamicTableData(uint16_t pointCount) {
    auto tableData = std::make_unique<DynamicSizeTableData>();
    tableData->m_Points.resize(pointCount);  // Resize vector to required number of points
    return tableData;
}

void stackAllocationBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        FixedSizeTableData tableData;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << std::setw(55) << std::left << "Pure stack allocation took: " << std::fixed << std::setprecision(10) << elapsed.count() << " seconds\n";
}

void stackWithDynamicVectorBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        DynamicSizeTableData tableData;
        tableData.m_Points.resize(2);  // Only allocate for 2 points
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << std::setw(55) << std::left << "Stack allocation with dynamic vector resizing took: " << std::fixed << std::setprecision(10) << elapsed.count() << " seconds\n";
}

void dynamicAllocationBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        auto tableData = createDynamicTableData(2);  // Only allocate for 2 points
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << std::setw(55) << std::left << "Dynamic allocation with unique_ptr took: " << std::fixed << std::setprecision(10) << elapsed.count() << " seconds\n";
}

int main() {
	/*
	const struct rlimit rl {
		.rlim_cur = 1L * 1024L * 1024L
	};
*/
	struct rlimit rl;
	getrlimit(RLIMIT_STACK, &rl);
	rl.rlim_cur = 1L * 1024L * 1024L;
	int result = setrlimit(RLIMIT_STACK, &rl);
	if (result != 0) {
		fprintf(stderr, "setrlimit returned result = %d\n", result);
	}
    stackAllocationBenchmark();
    stackWithDynamicVectorBenchmark();
    dynamicAllocationBenchmark();
    return 0;
}

