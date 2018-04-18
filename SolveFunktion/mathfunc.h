#pragma once

#include "tsrandom.h"
#include <array>
#include <algorithm>
#include <stdexcept>


enum MathOperator : uint8_t
{
	PLUS_PARAMETER				  =  1,
	PLUS_RANDOM_NUMBER			  =  2,
	MINUS_PARAMETER				  =  3,
	MINUS_RANDOM_NUMBER			  =  4,
	MINUS_PARAMETER_REVERSED	  =  5,
	MINUS_RANDOM_NUMBER_REVERSED  =  6,
	MULTIPLY_PARAMETER			  =  7,
	MULTIPLY_RANDOM_NUMBER		  =  8,
	DIVIDE_PARAMETER			  =  9,
	DIVIDE_RANDOM_NUMBER		  = 10,
	DIVIDE_PARAMETER_REVERSED	  = 11,
	DIVIDE_RANDOM_NUMBER_REVERSED = 12,
	POW      =  13,
	ROOT     = 14,
	EXP      = 15,
	NLOG     = 16,
	LOG      = 17,
	MOD      = 18,
	FLOOR    = 19,
	CEIL     = 20,
	ROUND    = 21,
	SIN      = 22,
	COS      = 23,
	TAN      = 24,
	ASIN     = 25,
	ACOS     = 26,
	ATAN     = 27,
	ABS      = 28,
	AND      = 29,
	NAND     = 30,
	OR       = 31,
	NOR      = 32,
	XOR      = 33,
	XNOR     = 34,
	NOT      = 35
};

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
class MathFunction
{
#define NO_OPERATOR -1
#define END_OPERATOR -2

#define IS_START_INDEX(x) (x == startOperatorIndex)
#define IS_END_INDEX(x) (nextOperatorIndex[x] == END_OPERATOR)
	
#define MAX_PARAMETER_COUNT 64
#define MAX_OPERATOR_COUNT 127

	static_assert(P_SIZE <= MAX_PARAMETER_COUNT, "Too many parameters given");
	static_assert(F_SIZE <= MAX_OPERATOR_COUNT, "The function can't contain that many operators");

public:
	float offset;

	MathFunction()
	{
		tcRandom = FRandom::getTCRandom();
	}

	static void setMathFunctionSettings(const std::array<MathOperator, O_SIZE>& allowedOps, const int32_t minNumber, const int32_t maxNumber)
	{
		allowedOperators = allowedOps;
		minRandomNumber = minNumber;
		maxRandomNumber = maxNumber;
	}

	void randomize()
	{
		const uint8_t functionLength = static_cast<uint8_t>(FRandom::randomRange(1, F_SIZE, tcRandom));
		for (int32_t i = 0; i < functionLength; i++)
		{
			operatorType[i] = getRandomMathOperator();
		}
		for (int32_t i = 0; i < functionLength; i++)
		{
			randomNumber[i] = static_cast<T>(FRandom::randomRange(minRandomNumber, maxRandomNumber, tcRandom));
		}
		metaData.fill(0);
		for (int32_t i = 0; i < functionLength; i++)
		{
			const uint8_t parameterIndex = static_cast<uint8_t>(FRandom::randomRange(0, P_SIZE - 1, tcRandom));
			setParameterIndex(i, parameterIndex);
		}
		for (int8_t i = 0; i < functionLength - 1; i++)
		{
			nextOperatorIndex[i] = i + 1;
		}
		nextOperatorIndex[functionLength - 1] = END_OPERATOR;
		for (int32_t i = functionLength; i < F_SIZE; i++)
		{
			nextOperatorIndex[i] = NO_OPERATOR;
		}
		operatorCount = functionLength;
		startOperatorIndex = 0;
	}

	bool calculate(const std::array<std::array<T, R_SIZE>, P_SIZE> & parameters, 
				   std::array<T, R_SIZE>& results, 
				   const std::array<T, R_SIZE>& expectedResults, 
				   const std::array<T, R_SIZE>& reciprocalExpectedResults, 
				   const std::array<std::array<T, R_SIZE>, P_SIZE>& reciprocalParameters)
	{
		std::copy(std::begin(parameters[0]), std::end(parameters[0]), std::begin(results));

		int8_t operatorIndex = startOperatorIndex;
		do
		{
			const int32_t parameterIndex = getParameterIndex(operatorIndex);
			bool success = true;
			switch (operatorType[operatorIndex])
			{
			case MathOperator::PLUS_PARAMETER:
				calculatePlus(parameters[parameterIndex], results);
				break;
			case MathOperator::PLUS_RANDOM_NUMBER:
				calculatePlus(results, randomNumber[operatorIndex]);
				break;
			case MathOperator::MINUS_PARAMETER:
				calculateMinus(parameters[parameterIndex], results);
				break;
			case MathOperator::MINUS_RANDOM_NUMBER:
				calculateMinus(results, randomNumber[operatorIndex]);
				break;
			case MathOperator::MINUS_PARAMETER_REVERSED:
				calculateMinusReversed(parameters[parameterIndex], results);
				break;
			case MathOperator::MINUS_RANDOM_NUMBER_REVERSED:
				calculateMinusReversed(results, randomNumber[operatorIndex]);
				break;
			case MathOperator::MULTIPLY_PARAMETER:
				calculateMultiply(parameters[parameterIndex], results);
				break;
			case MathOperator::MULTIPLY_RANDOM_NUMBER:
				calculateMultiply(results, randomNumber[operatorIndex]);
				break;
			case MathOperator::DIVIDE_PARAMETER:
				success = calculateDivide(parameters[parameterIndex], results);
				if (!success)
				{
					goto failedDivision;
				}
				break;
			case MathOperator::DIVIDE_RANDOM_NUMBER:
				success = calculateDivide(results, randomNumber[operatorIndex]);
				if (!success)
				{
					goto failedDivision;
				}
				break;
			case MathOperator::DIVIDE_PARAMETER_REVERSED:
				success = calculateDivideReversedReciprocal(reciprocalParameters[parameterIndex], results);
				if (!success)
				{
					goto failedDivision;
				}
				break;
			case MathOperator::DIVIDE_RANDOM_NUMBER_REVERSED:
				success = calculateDivideReversed(results, randomNumber[operatorIndex]);
				if (!success)
				{
					goto failedDivision;
				}
				break;
            default:
                break;
			}
						
			operatorIndex = nextOperatorIndex[operatorIndex];
		} while (operatorIndex != END_OPERATOR);

		offset = 0;
		for (uint32_t i = 0; i < R_SIZE; i++)
		{
			offset += std::abs((expectedResults[i] - results[i]) * reciprocalExpectedResults[i]);
		}
		return true;

	failedDivision:
		offset = 1000000;
		return false;
	}

	void copyTo(MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE> &copyInto) const
	{
		copyInto.operatorType = operatorType;
		copyInto.randomNumber = randomNumber;
		copyInto.metaData = metaData;
		copyInto.nextOperatorIndex = nextOperatorIndex;
		copyInto.offset = offset;
		copyInto.operatorCount = operatorCount;
		copyInto.startOperatorIndex = 0;
	}

	void sortedCopyTo(MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE> &copyInto) const
	{
		int8_t index = startOperatorIndex;
		for (int8_t copyInsertionIndex = 0; copyInsertionIndex < operatorCount; copyInsertionIndex++)
		{
			copyInto.operatorType[copyInsertionIndex] = operatorType[index];
			copyInto.randomNumber[copyInsertionIndex] = randomNumber[index];
			copyInto.metaData[copyInsertionIndex] = metaData[index];
			copyInto.nextOperatorIndex[copyInsertionIndex] = copyInsertionIndex + 1;

			index = nextOperatorIndex[index];
		}
		copyInto.nextOperatorIndex[operatorCount - 1] = END_OPERATOR;
		for (int32_t i = operatorCount; i < F_SIZE; i++)
		{
			copyInto.nextOperatorIndex[i] = NO_OPERATOR;
		}
		copyInto.offset = offset;
		copyInto.operatorCount = operatorCount;
		copyInto.startOperatorIndex = startOperatorIndex;
	}

	void evolve(const int32_t maxEvolve)
	{
		const int32_t evolveCount = FRandom::randomRange(1, maxEvolve, tcRandom);
		if (FRandom::randomBool(tcRandom))
		{
			for (int32_t i = 0; i < evolveCount; i++)
			{
				switch (FRandom::randomRange(0, 2, tcRandom))
				{
				case 0:
					if (canRemoveOperator())
					{
						removeOperator();
					}
					break;
				case 1:
					changeOperator();
					break;
				case 2:
					if (canAddOperator())
					{
						addOperator();
					}
					break;
				}
			}
		}
		else
		{
			if (FRandom::randomBool(tcRandom))
			{
				for (int32_t i = 0; i < evolveCount; i++)
				{
					changeOperatorNumber();
				}
			}
			else
			{
				for (int32_t i = 0; i < evolveCount; i++)
				{
					incrementChangeOperatorNumber();
				}
			}
		}
	}

private:
	const static uint32_t PARAMETER_INDEX_SHIFT = 2;
	const static uint32_t PARAMETER_INDEX_MASK = 0b1111'1100;

	static int32_t minRandomNumber;
	static int32_t maxRandomNumber;
	static std::array<MathOperator, O_SIZE> allowedOperators;

	FRandom::TCRandom tcRandom;
	int8_t operatorCount = 0;
	int8_t startOperatorIndex = 0;
	std::array<T, F_SIZE> randomNumber;
	std::array<MathOperator, F_SIZE> operatorType;
	std::array<uint8_t, F_SIZE> metaData;
	std::array<int8_t, F_SIZE> nextOperatorIndex;


	inline int32_t getParameterIndex(const int32_t index)
	{
		//return (metaData[index] & PARAMETER_INDEX_MASK) >> PARAMETER_INDEX_SHIFT;
		return metaData[index];
	}
	inline void setParameterIndex(const int32_t index, const int32_t value)
	{
		//metaData[index] |= (PARAMETER_INDEX_MASK & (value << PARAMETER_INDEX_SHIFT)):
		metaData[index] = value;
	}

	inline MathOperator getRandomMathOperator()
	{
		return allowedOperators[FRandom::randomRange(0, O_SIZE - 1, tcRandom)];
	}

	inline int32_t getRandomOperatorIndex()
	{
		int32_t index = FRandom::randomRange(0, F_SIZE - 1, tcRandom);
		while (nextOperatorIndex[index] == NO_OPERATOR)
		{
			index = FRandom::randomRange(0, F_SIZE - 1, tcRandom);
		}
		return index;
	}

	inline int32_t getIndexToNumber(const int numberToFind)
	{
		auto position = std::find(nextOperatorIndex.begin(), nextOperatorIndex.end(), numberToFind);
		if (position == nextOperatorIndex.end())
		{
			throw std::runtime_error("failed to find the value");
		}

		return position - nextOperatorIndex.begin();
	}

	inline bool canRemoveOperator()
	{
		return operatorCount > 1;
	}
	void removeOperator()
	{
		//find index to operator that will be removed
		const int32_t index = getRandomOperatorIndex();

		if (IS_START_INDEX(index))
		{
			//when removing the first operator the next operator in the chain will
			//be the start operator
			startOperatorIndex = nextOperatorIndex[index];
		}
		else
		{
			//we know the operator isn't the start operator so it has to have an operator
			//that points to this operator
			int32_t previousOperatorIndex = getIndexToNumber(index);

			if (IS_END_INDEX(index))
			{
				//if the end operator is removed then the operator that points to the end operator
				//needs to be the end operator
				nextOperatorIndex[previousOperatorIndex] = END_OPERATOR;
			}
			else
			{
				//as the operator isn't the start or end operator we know that the operator can be removed
				//by sending the previous operator to what this operator is pointing to.
				//that way this operator will be ignored
				nextOperatorIndex[previousOperatorIndex] = nextOperatorIndex[index];
			}
		}
		nextOperatorIndex[index] = NO_OPERATOR;


		operatorCount--;
	}

	void changeOperator()
	{
		const int32_t index = getRandomOperatorIndex();

		randomizeOperator(index);
	}

	inline bool canAddOperator()
	{
		return operatorCount < F_SIZE && operatorCount > 0;
	}
	void addOperator()
	{
		//find index to place new operator
		const int32_t index = getIndexToNumber(NO_OPERATOR);
		// generate new operator
		randomizeOperator(index);

		//now need to find an operator to attach to
		const int32_t insertionsPlace = getRandomOperatorIndex();
		
		//now make insertionsPlace point to index and index point to whatever insertionsPlace was pointing at
		nextOperatorIndex[index] = nextOperatorIndex[insertionsPlace];
		nextOperatorIndex[insertionsPlace] = static_cast<int8_t>(index);

		operatorCount++;
	}

	void randomizeOperator(const int index)
	{
		operatorType[index] = getRandomMathOperator();
		randomNumber[index] = static_cast<T>(FRandom::randomRange(minRandomNumber, maxRandomNumber, tcRandom));
		setParameterIndex(index, FRandom::randomRange(0, P_SIZE - 1, tcRandom));
	}

	void changeOperatorNumber()
	{
		const int32_t index = getRandomOperatorIndex();
		randomNumber[index] = static_cast<T>(FRandom::randomRange(minRandomNumber, maxRandomNumber, tcRandom));
	}

	void incrementChangeOperatorNumber()
	{
		const int32_t index = getRandomOperatorIndex();
		randomNumber[index] += (FRandom::randomBool(tcRandom)) ? 1 : -1;
	}
};

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
int32_t MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE>::minRandomNumber;

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
int32_t MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE>::maxRandomNumber;

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
std::array<MathOperator, O_SIZE> MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE>::allowedOperators;
