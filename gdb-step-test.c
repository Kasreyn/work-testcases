#include <stdio.h>
#include <stdbool.h>

bool f() {
	return true;
}

int main() {
	bool b1, b2;

	b1 = true;

	if (b1 && f()) {
		printf("b1 && b2\n");
	}

	if (b1) {
		printf("b1\n");
	}

	if (b2) {
		printf("b2\n");
	}
}
