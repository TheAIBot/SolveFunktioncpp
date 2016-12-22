// SolveFunktion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdint>
#include <cmath>
#include <time.h>
#include <thread>
#include <random>
#include <atomic>
#include <locale>
#include <mutex> 
#include "omath.h"
#include "tsrandom.h"
#include "mathfunc.h"

#define PARAMETERS_VALUE {{ 1,2,3,4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24, 25 }}
#define EXPECTED_RESULT_VALUE { 2,3,5,7,11,13,17,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101 }

#define FUNCTION_NUMBER_TYPE float
#define RESULT_LENGTH 25
#define FUNCTION_LENGTH 20
#define ALLOWED_OPS_LENGTH 4
#define DIFFERENT_PARAMETERS_COUNT 1


inline int64_t timediff(clock_t t1, clock_t t2) {
	return static_cast<int64_t>(((t2 - t1) * 1000) / CLOCKS_PER_SEC);
}

static std::atomic_int64_t timesDone = 0;
static std::atomic_int64_t randomFunctions = 0;
static std::mutex lockFoundNewBest;
static FUNCTION_NUMBER_TYPE bestResults[RESULT_LENGTH];
//bla bla bla thread safety bla bla bla
static float bestOffset = 1000000;

template<typename T, uint32_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
void getLeastOffset(const T(&parameters)[P_SIZE][R_SIZE], const T(&expectedResults)[R_SIZE], const MathOperator(&allowedOps)[O_SIZE])
{
	T results[R_SIZE] = { 0 };
	T thisBestResults[R_SIZE] = { 0 };

	MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE> bestFunc(allowedOps);
	MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE> testFunc(allowedOps);

	bestFunc.randomize();
	bestFunc.calculate(parameters, results, expectedResults);
	testFunc.randomize();
	randomFunctions++;

	clock_t startTime = clock();
#define STUCK_COUNT_FOR_RESET 1000000
	int32_t stuckCounter = 0;
	while (1)
	{
		testFunc.evolve(5);
		testFunc.calculate(parameters, results, expectedResults);
		if (testFunc.offset < bestFunc.offset)
		{
			testFunc.makeSortedCopy(bestFunc);
			stuckCounter = 0;
			for (int32_t i = 0; i < R_SIZE; i++)
			{
				thisBestResults[i] = results[i];
			}
		}
		else
		{
			bestFunc.makeCopy(testFunc);
		}

		if (stuckCounter == STUCK_COUNT_FOR_RESET)
		{
			testFunc.randomize();
			stuckCounter = 0;
			randomFunctions++;
		}
		else
		{
			stuckCounter++;
		}
		timesDone++;

		int64_t passedTime = timediff(startTime, clock());
		if (passedTime > 1000)
		{
			if (bestOffset > bestFunc.offset)
			{
				lockFoundNewBest.lock();

				bestOffset = bestFunc.offset;
				for (int32_t i = 0; i < R_SIZE; i++)
				{
					bestResults[i] = thisBestResults[i];
				}

				lockFoundNewBest.unlock();
			}
			startTime = clock();
		}
	}
}

int main()
{
	//this add a 1000 seperator so 1000 -> 1.000
	std::cout.imbue(std::locale(""));

	//this adds two deciamls to every floating value so 3,3423 -> 3,34
	std::cout << std::fixed;
	std::cout.precision(2);

	const FUNCTION_NUMBER_TYPE parameters[DIFFERENT_PARAMETERS_COUNT][RESULT_LENGTH] = PARAMETERS_VALUE;
	const FUNCTION_NUMBER_TYPE expectedResults[RESULT_LENGTH] = EXPECTED_RESULT_VALUE;
	const MathOperator allowedOps[ALLOWED_OPS_LENGTH] = {PLUS, MINUS, MULTIPLY, DIVIDE, /*POW, ROOT*/};



	std::cout << sizeof(MathFunction<FUNCTION_NUMBER_TYPE, FUNCTION_LENGTH, ALLOWED_OPS_LENGTH, RESULT_LENGTH, DIFFERENT_PARAMETERS_COUNT>) << std::endl;


	std::vector<std::thread> threads(std::thread::hardware_concurrency() - 5);

	int32_t threadCount = std::thread::hardware_concurrency() - 5;
	//int32_t threadCount = 1;
	for (int32_t i = 0; i < threadCount; i++)
	{
		threads[i] = std::thread(getLeastOffset<FUNCTION_NUMBER_TYPE, FUNCTION_LENGTH, ALLOWED_OPS_LENGTH, RESULT_LENGTH, DIFFERENT_PARAMETERS_COUNT>, std::cref(parameters), std::cref(expectedResults), std::cref(allowedOps));
	}


	clock_t startTime = clock();

	while (1)
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1s);
		

		int64_t passedTime = timediff(startTime, clock());
		int64_t correctedTimes = timesDone - static_cast<int64_t>(static_cast<float>(timesDone) * (1 - (static_cast<float>(passedTime) / 1000)));
		
		
		std::cout << passedTime << std::endl;
		std::cout << "Times: " << correctedTimes << std::endl;
		std::cout << "Failed: " << MathFunction<FUNCTION_NUMBER_TYPE, FUNCTION_LENGTH, ALLOWED_OPS_LENGTH, RESULT_LENGTH, DIFFERENT_PARAMETERS_COUNT>::failedCalculations << std::endl;
		MathFunction<FUNCTION_NUMBER_TYPE, FUNCTION_LENGTH, ALLOWED_OPS_LENGTH, RESULT_LENGTH, DIFFERENT_PARAMETERS_COUNT>::failedCalculations = 0;
		std::cout << "Offset: " << bestOffset << std::endl;
		std::cout << "Functions: " << randomFunctions << std::endl;
		lockFoundNewBest.lock();
		for (int32_t i = 0; i < RESULT_LENGTH; i++)
		{
			std::cout << bestResults[i] << ", ";
		}
		lockFoundNewBest.unlock();
		std::cout << std::endl;
		for (int32_t i = 0; i < RESULT_LENGTH; i++)
		{
			std::cout << expectedResults[i] << ", ";
		}
		std::cout << std::endl;

		startTime = clock();
		timesDone = 0;
	}

	
	int i;
	std::cin >> i;
    return 0;
}

