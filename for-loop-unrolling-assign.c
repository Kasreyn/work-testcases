#include <stdint.h>
#include <stdbool.h>

struct Coordinate {
	float x;
	float y;
};

bool Interpolate(struct Coordinate* c) {
	// return (c->x == 1.0f);
	return false;
}

uint32_t FindFirstValidEntry(uint32_t startOuter,
					  uint32_t sizeOuter,
					  uint32_t startInner,
					  uint32_t sizeInner,
					  uint32_t blockWidth,
					  uint32_t blockHeight,
					  bool outerIsX,
					  int increment) {
	struct Coordinate coordinate;

	for (uint32_t i = startOuter; (increment == 1) ? (i < sizeOuter) : (i > sizeOuter); i+=increment) {
		for (uint32_t j = startInner; j < sizeInner; ++j) {
			coordinate.x = outerIsX ? i * blockWidth : j * blockWidth;
			coordinate.y = outerIsX ? j * blockHeight : i * blockHeight;
			if (Interpolate(&coordinate)) {
				return i;
			}
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {
	uint32_t ret;
	uint32_t low = 0;
	uint32_t high = 10000;

	ret = FindFirstValidEntry(0, high,    0, high, 32, 32, false, 1);
	ret = FindFirstValidEntry(0, high, high,    0, 32, 32, false, -1);
}
