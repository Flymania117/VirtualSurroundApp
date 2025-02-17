#pragma once
#include <cstdint>

class EQFilterCurve {
protected:
	float _aCoefficients[3];
	float _bCoefficients[3];
public:
	float& A_0 = _aCoefficients[0];
	float& A_1 = _aCoefficients[1];
	float& A_2 = _aCoefficients[2];
	float& B_0 = _bCoefficients[0];
	float& B_1 = _bCoefficients[1];
	float& B_2 = _bCoefficients[2];
	virtual void SetCoefficients(float frequency, float Q, float gain, uint32_t sampleRate) = 0;
};