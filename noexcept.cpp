#include <stdexcept>

int MyFunc() {
	throw std::runtime_error("");
}

int main() {
	static_assert(noexcept(MyFunc()), "Lambda must be noexcept");
}

