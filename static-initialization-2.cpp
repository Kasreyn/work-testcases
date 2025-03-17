#include <iostream>
#include <string>

extern void printFile1Greeting();  // Forward declaration from file1

namespace {

    std::string getName() {
        return "file2";
    }

    std::string name = getName();

    struct Printer {
        Printer() {
            std::cout << "Initializing Printer in file2" << std::endl;
            printFile1Greeting();  // Uses `greeting` from file1
        }
    };

    Printer printer;
}
