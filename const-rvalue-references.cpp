#include <iostream>

struct T{
	std::string str;
};

void f(T& t) {
	std::cout << "lvalue ref, str=" << t.str << std::endl;
}
void f(const T& t) { 
	std::cout << "const lvalue ref str=" << t.str << std::endl;
}
//void f(const T&&) = delete; //{ std::cout << "rvalue ref\n"; }
//void f(const T&&) { std::cout << "const rvalue ref\n"; }

T&& g1(T&& s) {
	s.str = "g1";
	return std::move(s);
}

T& g2(T&& s) {
	s.str = "g2";
	return s;
}

int main() {
    f(g1(T()));
}

