#include "stdafx.h"
#include <cstdint>
#include <cmath>


template<typename T, int32_t N>
inline void calculatePlus(T(&results)[N], const T randomNumber)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] += randomNumber;
	}
}
template<typename T, int32_t N>
inline void calculatePlus(const T(& __restrict parameters)[N], T(& __restrict results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] += parameters[i];
	}
}

template<typename T, int32_t N>
inline void calculateMinus(T(&results)[N], const T randomNumber)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = randomNumber - results[i];
	}
}
template<typename T, int32_t N>
inline void calculateMinusReversed(T(&results)[N], const T randomNumber)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] -= randomNumber;
	}
}
template<typename T, int32_t N>
inline void calculateMinus(const T(& __restrict parameters)[N], T(& __restrict results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = parameters[i] - results[i];
	}
}
template<typename T, int32_t N>
inline void calculateMinusReversed(const T(& __restrict parameters)[N], T(& __restrict results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] -= parameters[i];
	}
}

template<typename T, int32_t N>
inline void calculateMultiply(T(&results)[N], const T randomNumber)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] *= randomNumber;
	}
}
template<typename T, int32_t N>
inline void calculateMultiply(const T(& __restrict parameters)[N], T(& __restrict results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] *= parameters[i];
	}
}

template<typename T, int32_t N>
inline bool calculateDivide(T(&results)[N], const T randomNumber)
{
	//make sure to not divide by 0
	for (int32_t i = 0; i < N; i++)
	{
		if (results[i] == 0)
		{
			return false;
		}
	}

	for (int32_t i = 0; i < N; i++)
	{
		results[i] = randomNumber / results[i];
	}
	return true;
}
template<typename T, int32_t N>
inline bool calculateDivideReversed(T(&results)[N], const T randomNumber)
{
	if (randomNumber == 0)
	{
		return false;
	}
	for (int32_t i = 0; i < N; i++)
	{
		results[i] /= randomNumber;
	}
	return true;
}
template<typename T, int32_t N>
inline bool calculateDivide(const T(& __restrict parameters)[N], T(& __restrict results)[N])
{
	//make sure to not divide by 0
	for (int32_t i = 0; i < N; i++)
	{
		if (results[i] == 0)
		{
			return false;
		}
	}

	for (int32_t i = 0; i < N; i++)
	{
		results[i] = parameters[i] / results[i];
	}
	return true;
}
template<typename T, int32_t N>
inline bool calculateDivideReversed(const T(& __restrict parameters)[N], T(& __restrict results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] /= parameters[i];
	}
	return true;
}


template<typename T, int32_t N>
void calculatePow(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}
template<int32_t N>
void calculatePow(const float(&parameters)[N], float(&results)[N], const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::powf(randomNumber, results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::powf(results[i], randomNumber);
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::powf(parameters[i], results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::powf(results[i], parameters[i]);
			}
		}
	}
}


template<typename T, int32_t N>
void calculateRoot(T(&results)[N])
{
}
template<int32_t N>
void calculateRoot(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::sqrtf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateExp(T(&results)[N])
{
}
template<int32_t N>
void calculateExp(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::expf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateNLog(T(&results)[N])
{
}
template<int32_t N>
void calculateNLog(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::logf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateLog(T(&results)[N])
{
}
template<int32_t N>
void calculateLog(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::log10f(results[i]);
	}
}


template<int32_t N>
void calculateMod(const float(&parameters)[N], float(&results)[N], const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::fmodf(randomNumber, results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::fmodf(results[i], randomNumber);
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::fmodf(parameters[i], results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::fmodf(results[i], parameters[i]);
			}
		}
	}
}
template<typename T, int32_t N>
void calculateMod(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = randomNumber % results[i];
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = results[i] % randomNumber;
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = parameters[i] % results[i];
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = results[i] % parameters[i];
			}
		}
	}
}


template<typename T, int32_t N>
void calculateFloor(T(&results)[N])
{
}
template<int32_t N>
void calculateFloor(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::floorf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateCeil(T(&results)[N])
{
}
template<int32_t N>
void calculateCeil(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::ceilf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateRound(T(&results)[N])
{
}
template<int32_t N>
void calculateRound(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::roundf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateSin(T(&results)[N])
{
}
template<int32_t N>
void calculateSin(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::sinf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateCos(T(&results)[N])
{
}
template<int32_t N>
void calculateCos(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::cosf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateTan(T(&results)[N])
{
}
template<int32_t N>
void calculateTan(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::tanf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateASin(T(&results)[N])
{
}
template<int32_t N>
void calculateASin(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::asinf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateACos(T(&results)[N])
{
}
template<int32_t N>
void calculateACos(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::acosf(results[i]);
	}
}


template<typename T, int32_t N>
void calculateATan(T(&results)[N])
{
}
template<int32_t N>
void calculateATan(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::atanf(results[i]);
	}

}


template<typename T, int32_t N>
void calculateAbs(T(&results)[N])
{
}
template<int32_t N>
void calculateAbs(float(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::abs(results[i]);
	}
}


template<typename T, int32_t N>
void calculateAND(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = randomNumber & results[i];
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = results[i] & randomNumber;
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = parameters[i] & results[i];
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = results[i] & parameters[i];
			}
		}
	}
}
template<int32_t N>
void calculateAND(const float(&parameters)[N], float(&results)[N], const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateNAND(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(randomNumber & results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(results[i] & randomNumber);
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(parameters[i] & results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(results[i] & parameters[i]);
			}
		}
	}
}
template<int32_t N>
void calculateNAND(const float(&parameters)[N], float(&results)[N], const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateOR(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = randomNumber | results[i];
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = results[i] | randomNumber;
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = parameters[i] | results[i];
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = results[i] | parameters[i];
			}
		}
	}
}
template<int32_t N>
void calculateOR(const float(&parameters)[N], float(&results)[N], const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateNOR(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(randomNumber | results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(results[i] | randomNumber);
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(parameters[i] | results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(results[i] | parameters[i]);
			}
		}
	}
}
template<int32_t N>
void calculateNOR(const float(&parameters)[N], float(&results)[N], const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateXOR(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = randomNumber ^ results[i];
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = results[i] ^ randomNumber;
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = parameters[i] ^ results[i];
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = results[i] ^ parameters[i];
			}
		}
	}
}
template<int32_t N>
void calculateXOR(const float(&parameters)[N], float(&results)[N], const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateXNOR(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(randomNumber ^ results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(results[i] ^ randomNumber);
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(parameters[i] ^ results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = ~(results[i] ^ parameters[i]);
			}
		}
	}
}
template<int32_t N>
void calculateXNOR(const float(&parameters)[N], float(&results)[N], const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}

template<typename T, int32_t N>
void calculateNot(T(&results)[N])
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = ~results[i];
	}
}
template<int32_t N>
void calculateNot(float(&results)[N])
{
}