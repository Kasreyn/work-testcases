#include <iostream>
#include <tuple>
#include <variant>
#include <string>
#include <vector>

// using MyVariant = std::variant<int, double, std::string>;
using MyVariant = std::variant<int, double, char>;

template <typename T, typename Ts>
struct GenerateCombinations {
    static void generate(void (*func)(T, Ts)) {
        func(T(), Ts());
/*
		std::apply([&](auto&&... args) {
            (func(T(), args), ...);
        }, tpl);
*/
    }
};

template <typename T, typename... Ts>
struct VariantProcessorBase {
    template <typename T1, typename T2>
    static void registerFunc(T1, T2) {
        std::cout << "Registering default function for: " << typeid(T1).name() << " and " << typeid(T2).name() << '\n';
    }

    template <typename T1, typename T2>
    void process(T1, T2) const {
        std::cout << "Default process for types: " << typeid(T1).name() << " and " << typeid(T2).name() << '\n';
    }

    VariantProcessorBase() {
        (GenerateCombinations<T, Ts>::generate(registerFunc<T, Ts>) ...);
/*
			registerFunc<T, Ts>...
            [this](auto x, auto y) {
                this->registerFunc(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
            },
            std::make_tuple(0, 0.0, 'c')
			
        );
*/
    }


};

class CustomVisitor {
private:
    const VariantProcessorBase<int, double, char>& processor_;

public:
    CustomVisitor(const VariantProcessorBase<int, double, char>& processor) : processor_(processor) {}

    template <typename T1, typename T2>
    void operator()(const T1& lhs, const T2& rhs) const {
        processor_.process(lhs, rhs);
    }
};


int main() {
//    std::vector<MyVariant> variants = { 10, 3.14, std::string("hello") };
    std::vector<MyVariant> variants = { 10, 3.14, 'c' };
//    VariantProcessor processor;
	VariantProcessorBase<int, double, char> processor;

    // Apply the custom visitor to all combinations of variants
	/*
    for (const auto& var1 : variants) {
        for (const auto& var2 : variants) {
			std::visit(CustomVisitor(processor), var1, var2);
        }
    }
	*/

    return 0;
}

