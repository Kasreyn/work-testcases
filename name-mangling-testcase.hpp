#include <string>

class Person {
public:
    Person(const std::string& name) : name(name) {}

    void Setup();

private:
    std::string name;
};

