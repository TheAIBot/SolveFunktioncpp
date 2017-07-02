#pragma once

namespace FRandom {

	//_declspec(dllimport)
		int32_t randomRange(const int32_t min, const int32_t max, struct TCRandom &random);

	//_declspec(dllimport)
		bool randomBool(struct TCRandom &random);

	//_declspec(dllimport)
		struct TCRandom getTCRandom();

	struct TCRandom
	{
		uint64_t x;
		uint64_t y;
		uint64_t z;
	};
}