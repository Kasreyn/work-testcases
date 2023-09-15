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

uint32_t FindFirstValidEntry(
		uint32_t startOuter,
		uint32_t sizeOuter,
		uint32_t startInner,
		uint32_t sizeInner,
		uint32_t blockWidth,
		uint32_t blockHeight,
		bool aIsX,
		bool normal) {
	struct Coordinate coord;

	if (normal) {
		for (uint32_t a = startOuter; a < sizeOuter; ++a) {
			for (uint32_t b = startInner; b < sizeInner; ++b) {
				coord.x = aIsX ? a * blockWidth : b * blockWidth;
				coord.y = aIsX ? b * blockHeight : a * blockHeight;
				if (Interpolate(&coord)) {
					return a;
				}
			}
		}
	}
	else {
		for (uint32_t a = startOuter; a > sizeOuter; --a) {
			for (uint32_t b = startInner; b < sizeInner; ++b) {
				coord.x = aIsX ? a * blockWidth : b * blockWidth;
				coord.y = aIsX ? b * blockHeight : a * blockWidth;
				if (Interpolate(&coord)) {
					return a;
				}
			}
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {
	uint32_t ret;
	uint32_t low = 0;
	uint32_t high = 10000;

	ret = FindFirstValidEntry(0, high,    0, high, 32, 32, false, true);
	ret = FindFirstValidEntry(0, high, high,    0, 32, 32, false, false);
}
