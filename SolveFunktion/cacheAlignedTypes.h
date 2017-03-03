#pragma once

#define CACHE_LINE_SIZE 64

template<typename T>
struct OneDataPerCacheLine {
	alignas(CACHE_LINE_SIZE)T data;
	char pad[CACHE_LINE_SIZE > sizeof(T) ? CACHE_LINE_SIZE - sizeof(T) : 1];
};