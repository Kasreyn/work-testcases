#include <fstream>
#include <iostream>
#include <tuple>

class Resource {
	public:
		std::string s = "Resource";
};

class Test {
	public:
		Test() = default;
		std::pair<Resource, bool> ReturnTest() {
			return std::make_pair(Resource(), false);
		}

};

int main(int argc, char *argv[]) {
	Test t;
	Resource r;
	bool b;
	std::pair<Resource, bool> t2 = t.ReturnTest();
	std::tie<Resource, bool>(r,b) = t.ReturnTest();
	std::cout << "Test: " << r.s << std::endl;
}
