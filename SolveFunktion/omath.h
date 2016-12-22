#pragma once

template<typename T, int32_t N>
void calculatePlus(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber);

template<typename T, int32_t N>
void calculateMinus(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateMultiply(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber);

template<typename T, int32_t N>
bool calculateDivide(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculatePow(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateRoot(T(&results)[N]);

template<typename T, int32_t N>
void calculateExp(T(&results)[N]);

template<typename T, int32_t N>
void calculateNLog(T(&results)[N]);

template<typename T, int32_t N>
void calculateLog(T(&results)[N]);

template<typename T, int32_t N>
void calculateMod(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateFloor(T(&results)[N]);

template<typename T, int32_t N>
void calculateCeil(T(&results)[N]);

template<typename T, int32_t N>
void calculateRound(T(&results)[N]);

template<typename T, int32_t N>
void calculateSin(T(&results)[N]);

template<typename T, int32_t N>
void calculateCos(T(&results)[N]);

template<typename T, int32_t N>
void calculateTan(T(&results)[N]);

template<typename T, int32_t N>
void calculateASin(T(&results)[N]);

template<typename T, int32_t N>
void calculateACos(T(&results)[N]);

template<typename T, int32_t N>
void calculateATan(T(&results)[N]);

template<typename T, int32_t N>
void calculateAbs(T(&results)[N]);

template<typename T, int32_t N>
void calculateAND(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateNAND(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateOR(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateNOR(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateXOR(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateXNOR(const T(&parameters)[N], T(&results)[N], const T randomNumber, const bool useRandomNumber, const bool resultOnRight);

template<typename T, int32_t N>
void calculateNot(T(&results)[N]);

//wtf c++?
#include "omath.cpp"