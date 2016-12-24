#pragma once

template<typename T, int32_t SIZE>
class ArrayAverage
{
private:
	T items[SIZE] = { 0 };
	int32_t index = 0;
	int32_t average = 0;

public:
	T insert(T item)
	{
		index = (index + 1) % SIZE;
		int32_t diff = item - items[index];
		items[index] = item;
		average += diff;
		return average / SIZE;
	}

};

