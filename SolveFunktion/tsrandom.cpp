#include <mutex> 
#include <cstdint>
#include "tsrandom.h"

namespace FRandom {
	static std::mutex initRandomLock;

	static uint64_t init_x = 236432356;
	static uint64_t init_y = 395824682;
	static uint64_t init_z = 443425897;

	struct TCRandom getTCRandom()
	{
		initRandomLock.lock();
		const struct TCRandom newRandom =
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

		const uint64_t t = random.x;
		random.x = random.y;
		random.y = random.z;
		random.z = t ^ random.x ^ random.y;

		return random.z;
	}

	int32_t randomRange(const int32_t min, const int32_t max, struct TCRandom &random)
	{
		const uint64_t z = fastRandom(random);

		return min + (z % static_cast<int32_t>(max - min + 1));
	}

	bool randomBool(struct TCRandom &random)
	{
		return fastRandom(random) & 1;
	}
}
