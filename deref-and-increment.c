#include <stdio.h>

int main (int argc, char *argv[]) {
	char *clock_pin = "PB10";
	char *pin;
	int port;

	if (*clock_pin == 'P')
		clock_pin++;

/*
	clock_pin++;
*/
	port = *clock_pin++ - 'A';
	printf("%s: port=%d clock_pin=%s\n", __func__, port, clock_pin);
}

