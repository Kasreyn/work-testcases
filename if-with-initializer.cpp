#include <iostream>
#include <utility>

std::pair<bool, int> getPair() {
    return {true, 42}; // returns a pair of bool and int
}

int main() {
    if (auto p = getPair(); p.first) {  // Accessing p.first for the condition
        std::cout << "Condition is true, value: " << p.second << std::endl;
    } else {
        std::cout << "Condition is false" << std::endl;
    }
}
