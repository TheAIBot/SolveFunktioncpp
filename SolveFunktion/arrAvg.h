#pragma once

#include <cstdint>

template<typename T, int32_t SIZE>
class ArrayAverage
{
private:
	T items[SIZE] = { 0 };
	int32_t index = 0;
	T average = 0;

public:
	T insert(const T item) 
	{
		index = (index + 1) % SIZE;
		const T diff = item - items[index];
		items[index] = item;
		average += diff;
		return average / SIZE;
	}
};