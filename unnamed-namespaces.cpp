#include <iostream>

// readelf --demangle -s -d -W unnamed-namespaces

namespace {
	int myVariable = 0;
}

int main() {
	std::cout << myVariable << std::endl;
}

