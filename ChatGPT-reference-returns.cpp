#include <iostream>
#include <map>
#include <functional>

class DummyObject {
public:
    DummyObject() : value(0) {} // Default constructor

    DummyObject(int value) : value(value) {}

    int getValue() const {
        return value;
    }

private:
    int value;
};

class ValueFinder {
public:
    ValueFinder() {
        // Use emplace to populate the internal map
        dataMap.emplace(1, 42);
        dataMap.emplace(2, 100);
        dataMap.emplace(3, 999);
    }

    std::pair<bool, std::reference_wrapper<const DummyObject>> findValue(int key) {
        auto it = dataMap.find(key);
        if (it != dataMap.end()) {
            return std::make_pair(true, std::cref(it->second)); // Return a reference wrapper to the found object
        } else {
            // Key not found, return a reference wrapper to the default-constructed DummyObject
            return std::make_pair(false, std::cref(dummy));
        }
    }

private:
    std::map<int, DummyObject> dataMap; // Using std::map
    DummyObject dummy; // Use the default constructor to create a dummy object
};

int main() {
    ValueFinder valueFinder; // Create the outer class in main

    std::pair<bool, std::reference_wrapper<const DummyObject>> result1 = valueFinder.findValue(2); // Try to find a value that exists
    if (result1.first) {
        const DummyObject& returnValue = result1.second; // Get the found object
        std::cout << "Found value: " << returnValue.getValue() << std::endl;
    } else {
        const DummyObject& returnValue = result1.second; // Get the default-constructed object
        std::cout << "Value not found. Default value: " << returnValue.getValue() << std::endl;
    }

    std::pair<bool, std::reference_wrapper<const DummyObject>> result2 = valueFinder.findValue(5); // Try to find a value that doesn't exist
    if (result2.first) {
        const DummyObject& returnValue = result2.second; // Get the found object
        std::cout << "Found value: " << returnValue.getValue() << std::endl;
    } else {
        const DummyObject& returnValue = result2.second; // Get the default-constructed object
        std::cout << "Value not found. Default value: " << returnValue.getValue() << std::endl;
    }

    return 0;
}
