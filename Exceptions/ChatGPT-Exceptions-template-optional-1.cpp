#include <iostream>
#include <optional>
#include <stdexcept>
#include <chrono>
#include <iomanip>


template<int Level>
std::optional<uint32_t> call_with_return_code() {
    if constexpr (Level == 0) {
        return std::nullopt;
    } else {
        std::optional<uint32_t> success = call_with_return_code<Level - 1>();
		if (success.has_value()) {
			return success.value();
		}
		else {
			return std::nullopt;
		}
    }
}

template<int Levels>
void test_return_optional_handling() {
    auto start = std::chrono::high_resolution_clock::now();

    if (!call_with_return_code<Levels>().has_value()) {
        std::cerr << "Error occurred at some level" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Return code handling time: " << duration.count() << " seconds" << std::endl;
}

int main() {
    const int levels = 890;

    std::cout << "\nTesting with optional return (" << levels << " levels):" << std::endl;
    test_return_optional_handling<levels>();
    
    return 0;
}
