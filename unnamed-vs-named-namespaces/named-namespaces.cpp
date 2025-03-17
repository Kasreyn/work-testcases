#include <iostream>

// readelf --demangle -s -d -W unnamed-namespaces

namespace MyNameSpace {
	int myVariable = 0;
}

int main() {
	std::cout << MyNameSpace::myVariable << std::endl;
}

