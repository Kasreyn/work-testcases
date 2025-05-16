#include <iostream>
#include <typeinfo>
#include <map>
#include <unordered_map>

class Animal {
public:
    virtual ~Animal() {
		std::cout << "~Animal()" << std::endl;
	}
};

class Dog : public Animal {
public:
	~Dog() {
		std::cout << "~Dog()" << std::endl;
	}
};

class Cat : public Animal {
public:
	~Cat() {
		std::cout << "~Cat()" << std::endl;
	}
};

int main() {
    std::unordered_map<uint8_t, Animal> typeObjectMap;

/*
    Dog dog;
    Cat cat;
*/
//	typeObjectMap.emplace(std::piecewise_construct, 0, ());
	typeObjectMap.emplace(0, Dog());

    return 0;
}

