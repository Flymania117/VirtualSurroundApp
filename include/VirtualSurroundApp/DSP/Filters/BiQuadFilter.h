#pragma once
#include "Filters.h"
#include <array>
#include <memory>

using namespace std;

class BiQuadFilter {
private:
	shared_ptr<EQFilterCurve> _curve;
	array<float, 3> _inputBuffer { 0, 0, 0 };
	array<float, 3> _outputBuffer { 0, 0, 0 };

public:
	BiQuadFilter() = default;
	BiQuadFilter(shared_ptr<EQFilterCurve> curve);
	~BiQuadFilter() = default;
	void SetCoefficients(float frequency, float Q, float gain, uint32_t sampleRate);
	float Process(float input);

	static BiQuadFilter CreateLowPass(float sampleRate, float cutoff, float q = 0.7071f);
	static BiQuadFilter CreateHighPass(float sampleRate, float cutoff, float q = 0.7071f);
	static BiQuadFilter CreateAllPass(float sampleRate, float cutoff, float q = 0.7071f);

};