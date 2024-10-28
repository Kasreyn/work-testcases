#include <variant>
#include <vector>
#include <iostream>

// Mock LibraryImpl class with the TransformCoordinates method
class LibraryImpl {
public:
    template <typename T, typename U>
    bool TransformCoordinates(int subscriptionID, const std::vector<T>& input, std::vector<std::vector<U>>& output) {
        std::cout << "TransformCoordinates called with types: " << typeid(T).name() << " and " << typeid(U).name() << std::endl;
        return true;
    }
};

// Template structure to instantiate combinations of types from std::variant
template <typename Variant>
struct InstantiateTransformCoordinates;

// Partial specialization for std::variant
template <typename... Types>
struct InstantiateTransformCoordinates<std::variant<Types...>> {
    // Function to instantiate for a pair of types
    template <typename T, typename U>
    static void instantiate() {
        LibraryImpl instance;
        int subscriptionID = 0;
        std::vector<T> input_vector;
        const std::vector<T> input_span = input_vector;
        std::vector<std::vector<U>> output;
        instance.TransformCoordinates(subscriptionID, input_span, output);
    }

    // Function to instantiate for all unique pairs of types from the variant
    static void instantiate_all() {
        // Instantiate for all combinations of types in the variant
        (instantiate<Types, Types>(), ...);  // Instantiate same-type combinations
        (instantiate<Types, Types>() , ...);  // Instantiate same-type combinations
    }
};

// Example usage
int main() {
    // Automatically deduce types from std::variant and instantiate combinations
    std::variant<int, double, char> var;
    InstantiateTransformCoordinates<decltype(var)>::instantiate_all();

    return 0;
}

