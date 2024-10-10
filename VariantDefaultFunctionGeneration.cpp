#include <iostream>
#include <variant>
#include <string>
#include <type_traits>
#include <vector>

// Define the variant type
using MyVariant = std::variant<int, double, std::string>;

// Helper metafunction to generate all combinations of variant types
template <typename T, typename... Ts>
struct GenerateCombinations {
    template <typename... Us>
    static void generate(void (*func)(T, Us...), std::tuple<Ts...>) {
        (func(T(), Ts()), ...);
    }
};

template <typename... Ts>
struct VariantProcessorBase {
    template <typename T1, typename T2>
    void process(T1, T2) const {
        std::cout << "Default process for types: " << typeid(T1).name() << " and " << typeid(T2).name() << '\n';
    }

    VariantProcessorBase() {
        GenerateCombinations<void>::generate(
            [this](auto x, auto y) {
                this->registerFunc(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
            },
            std::tuple<Ts...>()
        );
    }

    template <typename T1, typename T2>
    void registerFunc(T1, T2) const {
        std::cout << "Registering default function for: " << typeid(T1).name() << " and " << typeid(T2).name() << '\n';
    }
};

// Derived class to override specific combinations
struct VariantProcessor : VariantProcessorBase<int, double, std::string> {
    template <typename T1, typename T2>
    bool process(T1 lhs, T2 rhs) const {
        std::cout << "Processing: " << lhs << " and " << rhs << '\n';
        return true;
    }

    bool process(const int& lhs, const double& rhs) const {
        std::cout << "Processing int and double: " << lhs << ", " << rhs << '\n';
        return true;
    }

    bool process(const std::string& lhs, const int& rhs) const {
        std::cout << "Processing string and int: " << lhs << ", " << rhs << '\n';
        return true;
    }
};

// Custom visitor
class CustomVisitor {
public:
    CustomVisitor(const VariantProcessorBase<int, double, std::string>& processor) : processor_(processor) {}

    template <typename T1, typename T2>
    void operator()(const T1& lhs, const T2& rhs) const {
        processor_.process(lhs, rhs);
    }

private:
    const VariantProcessorBase<int, double, std::string>& processor_;
};

// Helper function to apply the custom visitor
void visit_custom(const MyVariant& var1, const MyVariant& var2, const VariantProcessorBase<int, double, std::string>& processor) {
    std::visit(CustomVisitor(processor), var1, var2);
}

int main() {
    std::vector<MyVariant> variants = { 10, 3.14, std::string("hello") };
    VariantProcessor processor;

    // Apply the custom visitor to all combinations of variants
    for (const auto& var1 : variants) {
        for (const auto& var2 : variants) {
            visit_custom(var1, var2, processor);
        }
    }

    return 0;
}

