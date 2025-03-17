
#include <cstdint>
#include <iostream>
#include <limits>

enum class Test {
	TEST0,
	TEST1,
	TEST2,
	TEST3
};

void ToString(Test v) {
	switch (v) {
		case Test::TEST1:
			std::cout << "TEST1" << std::endl;
			break;
		case Test::TEST2:
			std::cout << "TEST2" << std::endl;
			break;
	}
}

int main() {
	static constexpr auto MAX_PLUGINS = std::numeric_limits<uint8_t>::max() + 1;

	std::cout << "numeric_limits<uint8_t>::max() = " << (int)std::numeric_limits<uint8_t>::max() << std::endl;
	std::cout << "numeric_limits<int8_t>::max() = " << (int)std::numeric_limits<int8_t>::max() << std::endl;
	std::cout << "numeric_limits<int8_t>::min() = " << (int)std::numeric_limits<int8_t>::min() << std::endl;
	std::cout << "MAX_PLUGINS = " << (int)MAX_PLUGINS << std::endl;
	std::cout << +static_cast<uint8_t>(257) << std::endl;

	Test t = Test::TEST3;
	ToString(t);
}
