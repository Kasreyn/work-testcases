#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUF_GET_T(tgt, b) (memcpy(&tgt, b, sizeof(tgt)), sizeof(tgt))
#define BUF_GET_T1(tgt, b) (memcpy(&tgt, b, sizeof(tgt)))
#define BUF_GET_T2(tgt, b) (memcpy(&tgt, b, sizeof(tgt)), sizeof(tgt), (1234))

void main() {
	int stat_fd;
	int datapos;
	uint32_t shm_size;
	void *hdr_data = malloc(10);
	int t[] = { 0, 1 };
	void *data = malloc(20);

	
/*
	void *ret = BUF_GET_T(stat_fd, hdr_data);
*/
	int ret = BUF_GET_T2(stat_fd, hdr_data);
	printf("%s: ret=%d\n", __func__, ret);

	/* Fetch size of buffer */
	/*
	datapos += BUF_GET_T(shm_size, &data[datapos]);
	shm_size = le32toh(shm_size);
	*/
}
