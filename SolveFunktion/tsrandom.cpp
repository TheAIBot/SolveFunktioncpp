#include "stdafx.h"
#include <mutex> 
#include <cstdint>
#include "tsrandom.h"

static std::mutex initRandomLock;

static uint64_t init_x = 123456789;
static uint64_t init_y = 362436069;
static uint64_t init_z = 521288629;

struct TCRandom getTCRandom()
{
	initRandomLock.lock();
	struct TCRandom newRandom = 
	{ 
		init_x, 
		init_y, 
		init_z 
	};

	init_x++;
	init_y++;
	init_z++;
	initRandomLock.unlock();

	return newRandom;
}

static inline uint64_t fastRandom(struct TCRandom &random)
{
	random.x ^= random.x << 16;
	random.x ^= random.x >> 5;
	random.x ^= random.x << 1;

	uint64_t t = random.x;
	random.x = random.y;
	random.y = random.z;
	random.z = t ^ random.x ^ random.y;

	return random.z;
}

int32_t randomRange(const int32_t min, const int32_t max, struct TCRandom &random)
{
	const uint64_t z = fastRandom(random);

	return min + (z % (int32_t)(max - min + 1));
}

bool randomBool(struct TCRandom &random)
{
	return fastRandom(random) & 0x0000000000000001;
}