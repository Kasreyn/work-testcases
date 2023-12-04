#include <iostream>

int main() {
    int sum1 = 0;

    for (int i = 0; i < 1000000; ++i) {
        auto lambda = [&sum1]() {
            sum1++;
        };

        lambda(); // Call the lambda inside the loop
    }

    return 0;
}
