#pragma once
#include "Filters.h"
#include <array>

using namespace std;

class BiQuadFilter {
private:
	EQFilterCurve* _curve;
	array<float, 3> _inputBuffer { 0, 0, 0 };
	array<float, 3> _outputBuffer { 0, 0, 0 };

public:
	BiQuadFilter() = default;
	BiQuadFilter(EQFilterCurve* curve);
	~BiQuadFilter() = default;
	void SetCoefficients(float frequency, float Q, float gain, uint32_t sampleRate);
	float Process(float input);

	static BiQuadFilter CreateLowPass(float sampleRate, float cutoff, float q = 0.7071f);
	static BiQuadFilter CreateHighPass(float sampleRate, float cutoff, float q = 0.7071f);
	static BiQuadFilter CreateAllPass(float sampleRate, float cutoff, float q = 0.7071f);

};