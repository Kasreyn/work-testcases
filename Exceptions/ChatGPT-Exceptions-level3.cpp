#include <iostream>
#include <stdexcept>
#include <chrono>
#include <iomanip> 

void level3_ex() {
    throw std::runtime_error("Error at level 3");
}

void level2_ex() {
    level3_ex();
}

void level1_ex() {
    level2_ex();
}

bool level3() {
    return false;
}

bool level2() {
    if (!level3()) {
        return false;
    }
    return true;
}

bool level1() {
    if (!level2()) {
        return false;
    }
    return true;
}

void test_exception_handling() {
    auto start = std::chrono::high_resolution_clock::now();

	try {
		level1_ex();
	} catch (const std::runtime_error& e) {
		std::cerr << "Caught exception: " << e.what() << std::endl;
	}

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Exception handling time: " << std::fixed << std::setprecision(6) << duration.count() << " seconds" << std::endl;
}


void test_return_code_handling() {
    auto start = std::chrono::high_resolution_clock::now();

	if (!level1()) {
		std::cerr << "Error occurred at some level" << std::endl;
	}

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Return code handling time: " << std::fixed << std::setprecision(6) << duration.count() << " seconds" << std::endl;
}

int main() {
    std::cout << "Testing with exceptions:" << std::endl;
    test_exception_handling();

    std::cout << "\nTesting with return codes:" << std::endl;
    test_return_code_handling();

    return 0;
}
