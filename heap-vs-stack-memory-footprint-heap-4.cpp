#include <cstddef>

struct Point {
    double m_X;
};

struct TableData {
    Point m_Points[10];
};

int main() {
	TableData* t = new TableData;
	for (std::size_t i = 0; i < 10; ++i) {
		t->m_Points[i].m_X = 0.1;
	}
	while (true) {}
    return 0;
}
