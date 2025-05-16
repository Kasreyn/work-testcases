#include <iostream>
#include <optional>

class B {
	public:
		std::string m_Name = "B";
};

class A {
	public:
		std::string m_Name = "A";
		B& m_Bref;
		int m_Int;
		std::optional<int> m_Opt;
		static inline B m_BDefault;

		A() : m_Bref(m_BDefault), m_Int(5), m_Opt(std::nullopt) {
			std::cout << "Print(): " << m_Name << std::endl;
		}
};

int main(int argc, char *argv[]) {
	A a;
}
