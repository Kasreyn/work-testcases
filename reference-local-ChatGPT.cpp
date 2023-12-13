#include <iostream>

int& createReference(bool useLocal) {
    if (useLocal) {
        int localVariable = 42;
        return localVariable;  // Returning reference to a local variable
    } else {
        int* dynamicVariable = new int(100);
        return *dynamicVariable;  // Returning reference to dynamically allocated memory
    }
}

int main() {
    int& ref1 = createReference(true);
    int& ref2 = createReference(false);

    // The references 'ref1' and 'ref2' could potentially be dangling
    // if they refer to objects with limited scope or dynamic memory.

    // Accessing the references could lead to undefined behavior.

    std::cout << ref1 << std::endl;
    std::cout << ref2 << std::endl;

    // Note: In practice, compilers might issue warnings for such cases,
    // but detecting them reliably in all scenarios is challenging.

    return 0;
}
