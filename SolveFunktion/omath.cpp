#include <cstdint>
#include <cmath>
#include <array>


template<typename T, int32_t N>
inline void calculatePlus(std::array<T, N>& results, const T randomNumber)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] += randomNumber;
	}
}
template<typename T, int32_t N>
inline void calculatePlus(const std::array<T, N>& parameters, std::array<T, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] += parameters[i];
	}
}

template<typename T, int32_t N>
inline void calculateMinus(std::array<T, N>& results, const T randomNumber)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = randomNumber - results[i];
	}
}
template<typename T, int32_t N>
inline void calculateMinusReversed(std::array<T, N>& results, const T randomNumber)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] -= randomNumber;
	}
}
template<typename T, int32_t N>
inline void calculateMinus(const std::array<T, N>& parameters, std::array<T, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = parameters[i] - results[i];
	}
}
template<typename T, int32_t N>
inline void calculateMinusReversed(const std::array<T, N>& parameters, std::array<T, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] -= parameters[i];
	}
}

template<typename T, int32_t N>
inline void calculateMultiply(std::array<T, N>& results, const T randomNumber)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] *= randomNumber;
	}
}
template<typename T, int32_t N>
inline void calculateMultiply(const std::array<T, N>& parameters, std::array<T, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] *= parameters[i];
	}
}

template<typename T, int32_t N>
inline bool calculateDivide(std::array<T, N>& results, const T randomNumber)
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
inline bool calculateDivideReversed(std::array<T, N>& results, const T randomNumber)
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
inline bool calculateDivide(const std::array<T, N>& parameters, std::array<T, N>& results)
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
inline bool calculateDivideReversed(const std::array<T, N>& parameters, std::array<T, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] /= parameters[i];
	}
	return true;
}
template<typename T, int32_t N>
inline bool calculateDivideReversedReciprocal(const std::array<T, N>& reciprocalParameters, std::array<T, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] *= reciprocalParameters[i];
	}
	return true;
}


template<typename T, int32_t N>
void calculatePow(const std::array<T, N>& parameters, std::array<T, N>& results, const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}
template<int32_t N>
void calculatePow(const std::array<float, N>& parameters, std::array<float, N>& results, const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::pow(randomNumber, results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::pow(results[i], randomNumber);
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::pow(parameters[i], results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::pow(results[i], parameters[i]);
			}
		}
	}
}


template<typename T, int32_t N>
void calculateRoot(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateRoot(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::sqrt(results[i]);
	}
}


template<typename T, int32_t N>
void calculateExp(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateExp(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::exp(results[i]);
	}
}


template<typename T, int32_t N>
void calculateNLog(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateNLog(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::log(results[i]);
	}
}


template<typename T, int32_t N>
void calculateLog(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateLog(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::log10(results[i]);
	}
}


template<int32_t N>
void calculateMod(const std::array<float, N>& parameters, std::array<float, N>& results, const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
	if (useRandomNumber)
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::fmod(randomNumber, results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::fmod(results[i], randomNumber);
			}
		}
	}
	else
	{
		if (resultOnRight)
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::fmod(parameters[i], results[i]);
			}
		}
		else
		{
			for (int32_t i = 0; i < N; i++)
			{
				results[i] = std::fmod(results[i], parameters[i]);
			}
		}
	}
}
template<typename T, int32_t N>
void calculateMod(const std::array<T, N>& parameters, std::array<T, N>& results, const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
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
void calculateFloor(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateFloor(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::floor(results[i]);
	}
}


template<typename T, int32_t N>
void calculateCeil(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateCeil(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::ceil(results[i]);
	}
}


template<typename T, int32_t N>
void calculateRound(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateRound(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::round(results[i]);
	}
}


template<typename T, int32_t N>
void calculateSin(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateSin(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::sin(results[i]);
	}
}


template<typename T, int32_t N>
void calculateCos(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateCos(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::cos(results[i]);
	}
}


template<typename T, int32_t N>
void calculateTan(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateTan(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::tan(results[i]);
	}
}


template<typename T, int32_t N>
void calculateASin(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateASin(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::asin(results[i]);
	}
}


template<typename T, int32_t N>
void calculateACos(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateACos(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::acos(results[i]);
	}
}


template<typename T, int32_t N>
void calculateATan(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateATan(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::atan(results[i]);
	}

}


template<typename T, int32_t N>
void calculateAbs(std::array<T, N>& results)
{
}
template<int32_t N>
void calculateAbs(std::array<float, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = std::abs(results[i]);
	}
}


template<typename T, int32_t N>
void calculateAND(const std::array<T, N>& parameters, std::array<T, N>& results, const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
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
void calculateAND(const std::array<float, N>& parameters, std::array<float, N>& results, const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateNAND(const std::array<T, N>& parameters, std::array<T, N>& results, const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
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
void calculateNAND(const std::array<float, N>& parameters, std::array<float, N>& results, const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateOR(const std::array<T, N>& parameters, std::array<T, N>& results, const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
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
void calculateOR(const std::array<float, N>& parameters, std::array<float, N>& results, const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateNOR(const std::array<T, N>& parameters, std::array<T, N>& results, const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
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
void calculateNOR(const std::array<float, N>& parameters, std::array<float, N>& results, const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateXOR(const std::array<T, N>& parameters, std::array<T, N>& results, const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
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
void calculateXOR(const std::array<float, N>& parameters, std::array<float, N>& results, const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}


template<typename T, int32_t N>
void calculateXNOR(const std::array<T, N>& parameters, std::array<T, N>& results, const T randomNumber, const bool useRandomNumber, const bool resultOnRight)
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
void calculateXNOR(const std::array<float, N>& parameters, std::array<float, N>& results, const float randomNumber, const bool useRandomNumber, const bool resultOnRight)
{
}

template<typename T, int32_t N>
void calculateNot(std::array<T, N>& results)
{
	for (int32_t i = 0; i < N; i++)
	{
		results[i] = ~results[i];
	}
}
template<int32_t N>
void calculateNot(std::array<float, N>& results)
{
}