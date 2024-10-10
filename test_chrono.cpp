#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
	std::chrono::microseconds m_BusTimeout = std::chrono::seconds(1);
	std::cout << m_BusTimeout.count() << std::endl;

	return 0;
}
