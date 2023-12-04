#include <iostream>

class A {
	public:
		std::string m_Name = "A";
	public:
		void Print() {
			std::cout << "Print(): " << m_Name << std::endl;
		}
};

A& Test() {
	A a;
	return a;
}

int main(int argc, char *argv[]) {
	{
		A& a = Test();
		std::cout << a.m_Name << std::endl;
	}
	A& a2 = a;

	A* b = NULL;
	A& b2 = *b;
	std::cout << b2.m_Name << std::endl;
}
