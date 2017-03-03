#pragma once

int32_t randomRange(const int32_t min, const int32_t max, struct TCRandom &random);

bool randomBool(struct TCRandom &random);

struct TCRandom getTCRandom();

struct TCRandom
{
	uint64_t x;
	uint64_t y;
	uint64_t z;
};