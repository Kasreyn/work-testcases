#include <stdio.h>

typedef int capturemode_type;

int main(int argc, char **argv)
{
	capturemode_type capturemodes[] = {0, 0, 0, 0, 0, 0};

	printf("%s: sizeof(capturemodes)=%d\n", __func__, sizeof(capturemodes) / sizeof(capturemode_type));

	return 0;
}

