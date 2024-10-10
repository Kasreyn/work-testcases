// Translation Unit 2
#include "CommonHeader.hpp"
#include <typeindex>
#include <iostream>

int main() {
    const std::type_info& typeInfo = typeid(MyNamespace::MyType);
    std::type_index typeIndex(typeInfo);

    std::cout << typeIndex.hash_code() << std::endl;

    return 0;
}

