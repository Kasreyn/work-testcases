#include <stdio.h>

#define ALTDDR ALT1

int test_ALTDDR(void) {
	printf("%s\n", __func__);
}

int main(int argc, char **argv)
{
	printf("%s\n", __func__);

	test_ALTDDR();

	return 0;
}
