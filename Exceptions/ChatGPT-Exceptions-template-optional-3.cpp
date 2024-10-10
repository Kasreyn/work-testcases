#include <iostream>
#include <chrono>
#include <iomanip>

enum class ErrorCode {
	Something,
    Success,
    Failure
};

template<int Level>
ErrorCode call_with_return_code() {
    if constexpr (Level == 0) {
        return ErrorCode::Failure;
    } else {
        ErrorCode success = call_with_return_code<Level - 1>();
		if (success == ErrorCode::Failure) {
            return ErrorCode::Failure;
        }
		else {
			return ErrorCode::Success;
		}
    }
}

template<int Levels>
void test_return_code_handling() {
    auto start = std::chrono::high_resolution_clock::now();

    if (call_with_return_code<Levels>() == ErrorCode::Failure) {
        std::cerr << "Error occurred at some level" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Return code handling time: " << duration.count() << " seconds" << std::endl;
}

int main() {
    const int levels = 890;

    std::cout << "\nTesting with return codes (" << levels << " levels):" << std::endl;
    test_return_code_handling<levels>();
    
    return 0;
}
