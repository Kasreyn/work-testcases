#include <stdbool.h>
#include <stdio.h>

bool readout_imp_imp(int *level) {
 	*level = 11;       
}

bool readout_imp(int *level) {
	bool (*read) (int * level);
	read = *readout_imp_imp;
	
	return read(level);
}

bool readout(int *level) {
	return readout_imp(level);
}

int main (int argc, char *argv[]) {
	int level;
	
	readout(&level);
	printf("%s: level=%d\n", __func__, level);
}
