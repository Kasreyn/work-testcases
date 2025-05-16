#include <type_traits>

struct Inner {
    int x;
    void foo() {}  // Member function
/*
	Inner() {
	}
*/
};

struct Outer {
    Inner inner;
};

template<typename TTransformType>
concept ConceptTransformType = std::is_trivially_copyable_v<TTransformType> && std::is_standard_layout_v<TTransformType>;
// concept ConceptTransformType = std::is_trivial_v<TTransformType>;

template<ConceptTransformType TTransformType>
void RegisterTransformType() {
	TTransformType tt;
}

int main(int argc, char* argv[]) {
	RegisterTransformType<Outer>();
}
