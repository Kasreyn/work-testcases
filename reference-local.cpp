#include <iostream>

class A {
	public:
		std::string m_Name = "A";
};

A& GetA() {
	A a;
	return a;
}

int main(int argc, char *argv[]) {
	{
		A a;
	}
//	A& a_ref = a;

	A& a_ref_local = GetA();

	A a;
	A* b = nullptr;
	A& b2 = *b;
	std::cout << b2.m_Name << std::endl;
}
