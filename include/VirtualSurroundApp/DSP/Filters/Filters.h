#pragma once
#include "EQFilterCurve.h"

using namespace std;

class HighPassFilter : public EQFilterCurve {
public:
	HighPassFilter(float frequency, float Q, uint32_t sampleRate);
	void SetCoefficients(float frequency, float Q, float gain, uint32_t sampleRate) override;
};

class LowPassFilter : public EQFilterCurve {
public:
	LowPassFilter(float frequency, float Q, uint32_t sampleRate);
	void SetCoefficients(float frequency, float Q, float gain, uint32_t sampleRate) override;
};

class AllPassFilter : public EQFilterCurve {
public:
	AllPassFilter(float frequency, float Q, uint32_t sampleRate);
	void SetCoefficients(float frequency, float Q, float gain, uint32_t sampleRate) override;
};