#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

struct Point {
    float m_X;
    float m_Y;
};

struct TableData {
    uint16_t m_BlockWidth;
    uint16_t m_BlockHeight;
    uint16_t m_SizeX;
    uint16_t m_SizeY;
    uint16_t m_EntryCount;

    std::vector<Point> m_Points;  // Dynamic container
};

// Function that creates a TableData object with a dynamic number of points
std::unique_ptr<TableData> createTableData(uint16_t pointCount) {
    auto tableData = std::make_unique<TableData>();
    tableData->m_Points.resize(pointCount);  // Resize vector to required number of points
    return tableData;
}

void stackAllocationBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        TableData tableData;
        tableData.m_Points.resize(2);  // Only allocate for 2 points
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Stack allocation with dynamic vector resizing took: " << elapsed.count() << " seconds\n";
}

void dynamicAllocationBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        auto tableData = createTableData(2);  // Only allocate for 2 points
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Dynamic allocation with unique_ptr took: " << elapsed.count() << " seconds\n";
}

int main() {
    stackAllocationBenchmark();
    dynamicAllocationBenchmark();
    return 0;
}

