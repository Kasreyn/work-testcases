#include <iostream>

class B {
	public:
		std::string m_Name = "B";
};

class A {
	public:
		std::string m_Name = "A";
		B& m_Bref;
		static inline B m_BDefault;

		A() : m_Bref(m_BDefault) {
			std::cout << "Print(): " << m_Name << std::endl;
		}
};

int main(int argc, char *argv[]) {
	A a;
}
