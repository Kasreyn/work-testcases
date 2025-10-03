#include <vector>
#include <iostream>

// g++ -g -o output.s -fverbose-asm -S -fdiagnostics-color=always -std=c++20 -O0 GetCandidates-lambda.cpp
// g++ -g -o output.s.2 -fverbose-asm -S -fdiagnostics-color=always -std=c++20 -O2 GetCandidates-lambda.cpp

int square(int num) {
    struct Primitive {
        float a;
    };

    std::vector<Primitive> v;
	v.push_back(Primitive{0.1});
	v.push_back(Primitive{0.2});
	v.push_back(Primitive{0.3});

    const auto GetCandidates = [&v]() -> const std::vector<Primitive>& {
        return v;
    };

    for (const Primitive& p : GetCandidates()) {
        std::cout << p.a << std::endl;
    }

    for (const Primitive& p : v) {
        std::cout << p.a << std::endl;
    }

    return num * num;
}
