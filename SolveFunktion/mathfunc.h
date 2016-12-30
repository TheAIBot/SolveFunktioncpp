#pragma once


enum MathOperator
{
	PLUS = 0,
	MINUS = 1,
	MULTIPLY = 2,
	DIVIDE = 3,
	POW = 4,
	ROOT = 5,
	EXP = 6,
	NLOG = 7,
	LOG = 8,
	MOD = 9,
	FLOOR = 10,
	CEIL = 11,
	ROUND = 12,
	SIN = 13,
	COS = 14,
	TAN = 15,
	ASIN = 16,
	ACOS = 17,
	ATAN = 18,
	ABS = 19,
	AND = 20,
	NAND = 21,
	OR = 22,
	NOR = 23,
	XOR = 24,
	XNOR = 25,
	NOT = 26
};

template<typename T, uint32_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
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

#define GET_META_DATA(index, mask, shift) ((metaData[index] & mask) >> shift)
#define SET_META_DATA(index, value, mask, shift) (metaData[index] |= (mask & (value << shift)))

	static_assert(P_SIZE <= MAX_PARAMETER_COUNT, "Too many parameters given");

public:
	float offset;
	static std::atomic_int32_t failedCalculations;

	MathFunction(const MathOperator (&allowedOps)[O_SIZE], const int32_t minNumber, const int32_t maxNumber)
	{
		tcRandom = getTCRandom();
		for (uint32_t i = 0; i < O_SIZE; i++)
		{
			allowedOperators[i] = allowedOps[i];
		}
		minRandomNumber = minNumber;
		maxRandomNumber = maxNumber;
	}

	void randomize()
	{
		const int32_t functionLength = randomRange(1, F_SIZE, tcRandom);
		for (int32_t i = 0; i < functionLength; i++)
		{
			operatorType[i] = getRandomMathOperator();
		}
		for (int32_t i = 0; i < functionLength; i++)
		{
			randomNumber[i] = static_cast<T>(randomRange(minRandomNumber, maxRandomNumber, tcRandom));
		}
		memset(&metaData, 0, F_SIZE * sizeof(uint8_t));
		for (int32_t i = 0; i < functionLength; i++)
		{
			uint8_t randomMetaBools = randomRange(0, 3, tcRandom);
			setMeta(i, randomMetaBools);
		}
		for (int32_t i = 0; i < functionLength; i++)
		{
			uint8_t parameterIndex = randomRange(0, P_SIZE - 1, tcRandom);
			setParameterIndex(i, parameterIndex);
		}
		for (int32_t i = 0; i < functionLength - 1; i++)
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

	void calculate(const T(&parameters)[P_SIZE][R_SIZE], T(&results)[R_SIZE], const T(&expectedResults)[R_SIZE])
	{
		memcpy(&results, &parameters, R_SIZE * sizeof(T));
		//memset(&results, 0, R_SIZE * sizeof(T));

		int8_t operatorIndex = startOperatorIndex;
		do
		{
			const int32_t parameterIndex = getParameterIndex(operatorIndex);
			const uint8_t meta = getMeta(operatorIndex);
			const int32_t operatorNumber = static_cast<int32_t>(operatorType[operatorIndex]);
			const int32_t operatorVersionIndex = operatorNumber * 4 + meta;
			bool success = true;
			switch (operatorVersionIndex)
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
					offset = 1000000;
					failedCalculations++;
					return;
				}
				break;
			case 13:
				success = calculateDivideReversed(results, randomNumber[operatorIndex]);
				if (!success)
				{
					offset = 1000000;
					failedCalculations++;
					return;
				}
				break;
			case 14:
				success = calculateDivide(parameters[parameterIndex], results);
				if (!success)
				{
					offset = 1000000;
					failedCalculations++;
					return;
				}
				break;
			case 15:
				success = calculateDivideReversed(parameters[parameterIndex], results);
				if (!success)
				{
					offset = 1000000;
					failedCalculations++;
					return;
				}
				break;
			}
						
			operatorIndex = nextOperatorIndex[operatorIndex];
		} while (operatorIndex != END_OPERATOR);

		offset = 0;
		for (uint32_t i = 0; i < R_SIZE; i++)
		{
			offset += std::abs((expectedResults[i] - results[i]) / expectedResults[i]);
		}
	}

	void makeCopy(MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE> &copyInto)
	{
		memcpy(&copyInto.operatorType,		&operatorType,		F_SIZE * sizeof(MathOperator));
		memcpy(&copyInto.randomNumber,		&randomNumber,		F_SIZE * sizeof(T));
		memcpy(&copyInto.metaData,			&metaData,			F_SIZE * sizeof(uint8_t));
		memcpy(&copyInto.nextOperatorIndex, &nextOperatorIndex, F_SIZE * sizeof(int8_t));

		copyInto.offset = offset;
		copyInto.operatorCount = operatorCount;
		copyInto.startOperatorIndex = 0;
	}

	void makeSortedCopy(MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE> &copyInto)
	{
		uint32_t index = startOperatorIndex;
		for (uint32_t copyInsertionIndex = 0; copyInsertionIndex < operatorCount; copyInsertionIndex++)
		{
			copyInto.operatorType[copyInsertionIndex] = operatorType[index];
			copyInto.randomNumber[copyInsertionIndex] = randomNumber[index];
			copyInto.metaData[copyInsertionIndex] = metaData[index];
			copyInto.nextOperatorIndex[copyInsertionIndex] = copyInsertionIndex + 1;

			index = nextOperatorIndex[index];
		}
		copyInto.nextOperatorIndex[operatorCount - 1] = END_OPERATOR;
		//memset(&, NO_OPERATOR, )
		for (uint32_t i = operatorCount; i < F_SIZE; i++)
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
	MathOperator allowedOperators[O_SIZE];
	MathOperator operatorType[F_SIZE];
	T randomNumber[F_SIZE];
	uint8_t metaData[F_SIZE];
	int8_t nextOperatorIndex[F_SIZE];
	struct TCRandom tcRandom;
	uint32_t operatorCount = 0;
	uint32_t startOperatorIndex = 0;
	int32_t minRandomNumber;
	int32_t maxRandomNumber;

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

	inline int32_t getIndexToNumber(int numberToFind)
	{
		uint32_t index = 0;
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
		int32_t index = getRandomOperatorIndex();

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
		int32_t index = getRandomOperatorIndex();

		randomizeOperator(index);
	}

	inline bool canAddOperator()
	{
		return operatorCount < F_SIZE && operatorCount > 0;
	}
	void addOperator()
	{
		//find index to place new operator
		int32_t index = getIndexToNumber(NO_OPERATOR);
		// generate new operator
		randomizeOperator(index);

		//now need to find an operator to attach to
		int32_t insertionsPlace = getRandomOperatorIndex();
		
		//now make insertionsPlace point to index and index point to whatever insertionsPlace was pointing at
		nextOperatorIndex[index] = nextOperatorIndex[insertionsPlace];
		nextOperatorIndex[insertionsPlace] = index;

		operatorCount++;
	}

	void randomizeOperator(int index)
	{
		operatorType[index] = getRandomMathOperator();
		randomNumber[index] = static_cast<T>(randomRange(minRandomNumber, maxRandomNumber, tcRandom));
		setMeta(index, randomRange(0, 3, tcRandom));
		setParameterIndex(index, randomRange(0, P_SIZE - 1, tcRandom));
	}

	void changeOperatorNumber()
	{
		int32_t index = getRandomOperatorIndex();
		randomNumber[index] = static_cast<T>(randomRange(minRandomNumber, maxRandomNumber, tcRandom));
	}

	void incrementChangeOperatorNumber()
	{
		int32_t index = getRandomOperatorIndex();
		randomNumber[index] += (randomBool(tcRandom)) ? 1 : -1;
	}
};

template<typename T, uint32_t F_SIZE, uint32_t O_SIZE, uint32_t R_SIZE, uint32_t P_SIZE>
std::atomic_int32_t MathFunction<T, F_SIZE, O_SIZE, R_SIZE, P_SIZE>::failedCalculations = 0;