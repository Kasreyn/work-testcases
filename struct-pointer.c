#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h>

#define DATACACHE_MAX_NAME_LEN 100

struct datacache_create {
	char name[DATACACHE_MAX_NAME_LEN];
	unsigned int size;
	unsigned int entries;
};

/*
typedef struct DatacacheData {
	int dcCache = 0;
	int dcFd = 0;
	struct datacache_create bbDatacache;
} DatacacheData;
*/

struct DatacacheData {
	int dcCache;
	int dcFd;
	struct datacache_create bbDatacache;
};

void print(struct datacache_create *dc) {
	printf("%s: dc->size=%d\n", __func__, dc->size);
}

int main (int argc, char *argv[]) {
	struct DatacacheData datacache;
	struct DatacacheData *dc = &datacache;
	
	datacache.bbDatacache.size = 44;

/*
	ioctl(0, 0, &dc->bbDatacache);
*/

	print(&dc->bbDatacache);
}
