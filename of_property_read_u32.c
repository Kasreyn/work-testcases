#include <stdio.h>


int main(int argc, char **argv)
{
	int errno;

	errno = -1;
	if (errno)
		printf("%s: errno=%d\n", __func__, errno);

	errno = 1;
	if (errno)
		printf("%s: errno=%d\n", __func__, errno);

	errno = 0;
	if (errno)
		printf("%s: errno=%d\n", __func__, errno);

	return 0;
}
