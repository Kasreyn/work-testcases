#include <iostream>
#include <string>

namespace {
    std::string getGreeting() {
        return "Hello from file1";
    }

    std::string greeting = getGreeting();
}

void printFile1Greeting() {
    std::cout << greeting << std::endl;
}

