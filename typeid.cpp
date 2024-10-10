#include <iostream>
#include <typeinfo>
#include <map>

class Animal {
public:
    virtual ~Animal() {}
    virtual void makeSound() const = 0;
};

class Dog : public Animal {
public:
    void makeSound() const override {
        std::cout << "Woof!" << std::endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() const override {
        std::cout << "Meow!" << std::endl;
    }
};

int main() {
    std::map<const std::type_info*, Animal*> typeObjectMap;

    Dog dog;
    Cat cat;

    typeObjectMap[&typeid(Dog)] = &dog;
    typeObjectMap[&typeid(Cat)] = &cat;

    Animal* animalPtr = &dog;  // or &cat

    const std::type_info* typeInfo = &typeid(*animalPtr);

    // Check if the type exists in the map
    auto it = typeObjectMap.find(typeInfo);
    if (it != typeObjectMap.end()) {
        // Found the type in the map, now cast the base pointer to the correct type
        Animal* correspondingObject = it->second;

        // Perform dynamic_cast
        if (Dog* dogPtr = dynamic_cast<Dog*>(correspondingObject)) {
            std::cout << "It's a Dog!" << std::endl;
            dogPtr->makeSound();  // Calls makeSound() of the correct type
        } else if (Cat* catPtr = dynamic_cast<Cat*>(correspondingObject)) {
            std::cout << "It's a Cat!" << std::endl;
            catPtr->makeSound();  // Calls makeSound() of the correct type
        } else {
            std::cout << "Failed to cast to the correct type." << std::endl;
        }
    } else {
        std::cout << "Unknown animal type." << std::endl;
    }

    return 0;
}

