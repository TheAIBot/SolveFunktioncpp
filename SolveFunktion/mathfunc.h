#pragma once

#include "cacheAlignedTypes.h"


enum MathOperator : uint8_t
{
	PLUS     =  0,
	MINUS    =  1,
	MULTIPLY =  2,
	DIVIDE   =  3,
	POW      =  4,
	ROOT     =  5,
	EXP      =  6,
	NLOG     =  7,
	LOG      =  8,
	MOD      =  9,
	FLOOR    = 10,
	CEIL     = 11,
	ROUND    = 12,
	SIN      = 13,
	COS      = 14,
	TAN      = 15,
	ASIN     = 16,
	ACOS     = 17,
	ATAN     = 18,
	ABS      = 19,
	AND      = 20,
	NAND     = 21,
	OR       = 22,
	NOR      = 23,
	XOR      = 24,
	XNOR     = 25,
	NOT      = 26
};

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
class MathFunction
{
#define NO_OPERATOR -1
#define END_OPERATOR -2

#define IS_START_INDEX(x) (x == startOperatorIndex)
#define IS_END_INDEX(x) (nextOperatorIndex[x] == END_OPERATOR)
	
#define META_SHIFT 0
#define META_MASK 0x03
#define PARAMETER_INDEX_SHIFT 2
#define PARAMETER_INDEX_MASK 0xFC
#define MAX_PARAMETER_COUNT 64
#define MAX_OPERATOR_COUNT 127

#define GET_META_DATA(index, mask, shift) ((metaData[index] & mask) >> shift)
#define SET_META_DATA(index, value, mask, shift) (metaData[index] |= (mask & (value << shift)))

	static_assert(P_SIZE <= MAX_PARAMETER_COUNT, "Too many parameters given");
	static_assert(F_SIZE <= MAX_OPERATOR_COUNT, "The function can't contain that many operators");

public:
	float offset;

	MathFunction()
	{
		tcRandom = getTCRandom();
	}

	static void setMathFunctionSettings(const MathOperator(&allowedOps)[O_SIZE], const int32_t minNumber, const int32_t maxNumber)
	{
		std::copy(std::begin(allowedOps), std::end(allowedOps), std::begin(allowedOperators));
		minRandomNumber = minNumber;
		maxRandomNumber = maxNumber;
	}

	void randomize()
	{
		const uint8_t functionLength = static_cast<uint8_t>(randomRange(1, F_SIZE, tcRandom));
		for (int32_t i = 0; i < functionLength; i++)
		{
			operatorType[i] = getRandomMathOperator();
		}
		for (int32_t i = 0; i < functionLength; i++)
		{
			randomNumber[i] = static_cast<T>(randomRange(minRandomNumber, maxRandomNumber, tcRandom));
		}
		std::fill(std::begin(metaData), std::end(metaData), 0);
		for (int32_t i = 0; i < functionLength; i++)
		{
			const uint8_t randomMetaBools = static_cast<uint8_t>(randomRange(0, 3, tcRandom));
			setMeta(i, randomMetaBools);
		}
		for (int32_t i = 0; i < functionLength; i++)
		{
			const uint8_t parameterIndex = static_cast<uint8_t>(randomRange(0, P_SIZE - 1, tcRandom));
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

	bool calculate(const T(&parameters)[P_SIZE][R_SIZE], T(&results)[R_SIZE], const T(&expectedResults)[R_SIZE], const T(&reciprocalExpectedResults)[R_SIZE], const T(&reciprocalParameters)[P_SIZE][R_SIZE])
	{
		std::copy(std::begin(parameters[0]), std::end(parameters[0]), std::begin(results));

		int8_t operatorIndex = startOperatorIndex;
		do
		{
			const int32_t parameterIndex = getParameterIndex(operatorIndex);
			const uint8_t meta = getMeta(operatorIndex);
			const uint8_t operatorNumber = static_cast<uint8_t>(operatorType[operatorIndex]);
			const int32_t operatorVersionType = operatorNumber * 4 + meta;
			bool success = true;
			switch (operatorVersionType)
			{
			case 0:
			case 1:
				calculatePlus(results, randomNumber[operatorIndex]);
				break;
			case 2:
			case 3:
				calculatePlus(parameters[parameterIndex], results);
				break;
			case 4:
				calculateMinus(results, randomNumber[operatorIndex]);
				break;
			case 5:
				calculateMinusReversed(results, randomNumber[operatorIndex]);
				break;
			case 6:
				calculateMinus(parameters[parameterIndex], results);
				break;
			case 7:
				calculateMinusReversed(parameters[parameterIndex], results);
				break;
			case 8:
			case 9:
				calculateMultiply(results, randomNumber[operatorIndex]);
				break;
			case 10:
			case 11:
				calculateMultiply(parameters[parameterIndex], results);
				break;
			case 12:
				success = calculateDivide(results, randomNumber[operatorIndex]);
				if (!success)
				{
					goto failedDivision;
				}
				break;
			case 13:
				success = calculateDivideReversed(results, randomNumber[operatorIndex]);
				if (!success)
				{
					goto failedDivision;
				}
				break;
			case 14:
				success = calculateDivide(parameters[parameterIndex], results);
				if (!success)
				{
					goto failedDivision;
				}
				break;
			case 15:
				//success = calculateDivideReversed(parameters[parameterIndex], results);
				success = calculateDivideReversedReciprocal(reciprocalParameters[parameterIndex], results);
				if (!success)
				{
					goto failedDivision;
				}
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
		std::copy(std::begin(operatorType), std::end(operatorType), std::begin(copyInto.operatorType));
		std::copy(std::begin(randomNumber), std::end(randomNumber), std::begin(copyInto.randomNumber));
		std::copy(std::begin(metaData), std::end(metaData), std::begin(copyInto.metaData));
		std::copy(std::begin(nextOperatorIndex), std::end(nextOperatorIndex), std::begin(copyInto.nextOperatorIndex));

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
		const int32_t evolveCount = randomRange(1, maxEvolve, tcRandom);
		if (randomBool(tcRandom))
		{
			for (int32_t i = 0; i < evolveCount; i++)
			{
				switch (randomRange(0, 2, tcRandom))
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
			if (randomBool(tcRandom))
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
	static int32_t minRandomNumber;
	static int32_t maxRandomNumber;
	static MathOperator allowedOperators[O_SIZE];

	struct TCRandom tcRandom;
	T randomNumber[F_SIZE];
	int8_t operatorCount = 0;
	int8_t startOperatorIndex = 0;
	MathOperator operatorType[F_SIZE];
	uint8_t metaData[F_SIZE];
	int8_t nextOperatorIndex[F_SIZE];
	

	inline uint8_t getMeta(const int32_t index)
	{
		return GET_META_DATA(index, META_MASK, META_SHIFT);
	}
	inline void setMeta(const int32_t index, const uint8_t value)
	{
		SET_META_DATA(index, value, META_MASK, META_SHIFT);
	}

	inline int32_t getParameterIndex(const int32_t index)
	{
		return GET_META_DATA(index, PARAMETER_INDEX_MASK, PARAMETER_INDEX_SHIFT);
	}
	inline void setParameterIndex(const int32_t index, const int32_t value)
	{
		SET_META_DATA(index, value, PARAMETER_INDEX_MASK, PARAMETER_INDEX_SHIFT);
	}

	inline MathOperator getRandomMathOperator()
	{
		return allowedOperators[randomRange(0, O_SIZE - 1, tcRandom)];
	}

	inline int32_t getRandomOperatorIndex()
	{
		int32_t index = randomRange(0, F_SIZE - 1, tcRandom);
		while (nextOperatorIndex[index] == NO_OPERATOR)
		{
			index = randomRange(0, F_SIZE - 1, tcRandom);
		}
		return index;
	}

	inline int32_t getIndexToNumber(const int numberToFind)
	{
		int32_t index = 0;
		while (nextOperatorIndex[index] != numberToFind)
		{
			index++;
		}

		return index;
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
		nextOperatorIndex[insertionsPlace] = index;

		operatorCount++;
	}

	void randomizeOperator(const int index)
	{
		operatorType[index] = getRandomMathOperator();
		randomNumber[index] = static_cast<T>(randomRange(minRandomNumber, maxRandomNumber, tcRandom));
		setMeta(index, static_cast<uint8_t>(randomRange(0, 3, tcRandom)));
		setParameterIndex(index, randomRange(0, P_SIZE - 1, tcRandom));
	}

	void changeOperatorNumber()
	{
		const int32_t index = getRandomOperatorIndex();
		randomNumber[index] = static_cast<T>(randomRange(minRandomNumber, maxRandomNumber, tcRandom));
	}

	void incrementChangeOperatorNumber()
	{
		const int32_t index = getRandomOperatorIndex();
		randomNumber[index] += (randomBool(tcRandom)) ? 1 : -1;
	}
};

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
void MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE>::setMathFunctionSettings(const MathOperator(&allowedOps)[O_SIZE], const int32_t minNumber, const int32_t maxNumber);

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
int32_t MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE>::minRandomNumber;

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
int32_t MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE>::maxRandomNumber;

template<typename T, uint8_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
MathOperator MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE>::allowedOperators[O_SIZE];