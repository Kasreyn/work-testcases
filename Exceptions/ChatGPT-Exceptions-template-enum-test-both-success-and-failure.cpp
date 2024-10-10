#include <iostream>
#include <stdexcept>
#include <chrono>
#include <iomanip>

enum class ErrorCode {
	Something,
    Success,
    Failure
};

template<int Level, bool hasFailure>
void call_with_exception() {
    if constexpr (Level == 0 && hasFailure) {
        throw std::runtime_error("Error at the deepest level");
    } else {
        call_with_exception<Level - 1, hasFailure>();
    }
}

template<int Level, bool hasFailure>
ErrorCode call_with_return_code() {
    if constexpr (Level == 0 && hasFailure) {
        return ErrorCode::Failure;
    } else {
        ErrorCode success = call_with_return_code<Level - 1, hasFailure>();
		if (success == ErrorCode::Failure) {
            return ErrorCode::Failure;
        }
		else {
			return ErrorCode::Success;
		}
    }
}

template<int Levels, bool hasFailure>
void test_exception_handling() {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        call_with_exception<Levels, hasFailure>();
    } catch (const std::runtime_error& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Exception handling time: " << duration.count() << " seconds" << std::endl;
}

template<int Levels, bool hasFailure>
void test_return_code_handling() {
    auto start = std::chrono::high_resolution_clock::now();

    if (call_with_return_code<Levels, hasFailure>() == ErrorCode::Failure) {
        std::cerr << "Error occurred at some level" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Return code handling time: " << duration.count() << " seconds" << std::endl;
}

int main() {
    const int levels = 1;

    std::cout << "Failure path: Testing with exceptions (" << levels << " levels):" << std::endl;
    test_exception_handling<levels, true>();

    std::cout << "\nFailure path: Testing with return codes (" << levels << " levels):" << std::endl;
    test_return_code_handling<levels, true>();

    std::cout << "\nSuccess path: Testing with exceptions (" << levels << " levels):" << std::endl;
    test_exception_handling<levels, false>();

    std::cout << "\nSuccess path: Testing with return codes (" << levels << " levels):" << std::endl;
    test_return_code_handling<levels, false>();

    return 0;
}
