#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {

	auto Process = [] () {
		std::cout << "Hi" << std::endl;
	};

	std::thread thread(Process);
}
