/*
#include<iostream>
#include <string>
#include <tuple>

// helper function to print a tuple of any size
template<class Tuple, std::size_t N>
struct TuplePrinter {
	static void print(const Tuple& t) {
		TuplePrinter<Tuple, N - 1>::print(t);
		std::cout << ", " << std::get<N - 1>(t);
	}
};

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
	static void print(const Tuple& t) {
		std::cout << std::get<0>(t);
	}
};

template<class... Args>
void print(const std::tuple<Args...>& t) {
	std::cout << "(";
	TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
	std::cout << ")\n";
}

int main() {
	std::tuple<int, std::string, float> t1(10, "Test", 3.14);
	print(t1);
}
*/

#include <iostream>
#include <variant>
#include <type_traits>

using MyVariant = std::variant<int, double, char>;

template <typename T, typename U>
void __attribute__((used,retain,noinline)) test(T x, U y) {
    std::cout << "Function called with types: " << typeid(T).name() << " and " << typeid(U).name() << std::endl;
}

__attribute__((used,retain,noinline)) static void instantiate() {
    MyVariant x = 3.14, y = 2; // Example variant initialization

    // Visit the variant and create combinations
    std::visit([]<typename T, typename U>(T x, U y) {
        if constexpr (!std::is_same_v<T, U>) {
            test(x, y); // Call test if types are different
        }
    }, x, y);
}

int main() {
    instantiate();
    return 0;
}
