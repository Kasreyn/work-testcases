#include <iostream>

// readelf --demangle -s -d -W unnamed-namespaces

class Person {
	public:
	Person(const std::string& name) : name(name) {}

	void Setup() {
		std::cout << "Hello, my name is " << name << std::endl;
	}

	private:
	std::string name;
};

namespace {
	Person myPerson("Njure");
}

int main() {
	myPerson.Setup();
}

