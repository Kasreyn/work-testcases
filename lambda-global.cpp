#include <iostream>

int main() {
    int sum2 = 0;
    auto lambda = [&sum2]() {
        sum2++;
    };

    for (int i = 0; i < 1000000; ++i) {
        lambda(); // Call the lambda inside the loop
    }

    return 0;
}
