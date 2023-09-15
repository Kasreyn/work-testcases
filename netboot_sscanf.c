#include <stdio.h>

int main (int argc, char *argv[]) {
	unsigned long end = 1073741823;
	unsigned int ret;
	const char *s_end = "0x3fffffff";

	sscanf(s_end, "%x", &ret);
	end = ret;

	printf("%s: end=%lu\n", __func__, end);
}
