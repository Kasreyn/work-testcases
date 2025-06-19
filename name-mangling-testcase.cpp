#include <iostream>

// g++ -fPIC -shared -o libname-mangling-testcase.so name-mangling-testcase.cpp -Wl,-version-script,ct.map

class Person {
public:
    Person(const std::string& name) : name(name) {}

    void Setup() {
        std::cout << "Hello, my name is " << name << std::endl;
    }

private:
    std::string name;
};

int main() {
    Person person("John");
    person.Setup();

    return 0;
}

