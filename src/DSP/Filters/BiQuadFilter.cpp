#include "BiQuadFilter.h"
#include "Filters.h"
#include <array>

using namespace std;

BiQuadFilter::BiQuadFilter(EQFilterCurve* curve) {
	_curve = curve;
}

void BiQuadFilter::SetCoefficients(float frequency, float Q, float gain, uint32_t sampleRate) {
	_curve->SetCoefficients(frequency, Q, gain, sampleRate);
}

float BiQuadFilter::Process(float input) {
    _inputBuffer[2] = _inputBuffer[1];
    _inputBuffer[1] = _inputBuffer[0];
    _inputBuffer[0] = input;

    _outputBuffer[2] = _outputBuffer[1];
    _outputBuffer[1] = _outputBuffer[0];

    _outputBuffer[0] = _curve->B_0 / _curve->A_0 * _inputBuffer[0]
        + _curve->B_1 / _curve->A_0 * _inputBuffer[1]
        + _curve->B_2 / _curve->A_0 * _inputBuffer[2]
        - _curve->A_1 / _curve->A_0 * _outputBuffer[1]
        - _curve->A_2 / _curve->A_0 * _outputBuffer[2];

    return _outputBuffer[0];
}

BiQuadFilter BiQuadFilter::CreateLowPass(float sampleRate, float cutoff, float q) {
	return BiQuadFilter(new LowPassFilter(cutoff, q, sampleRate));
}

BiQuadFilter BiQuadFilter::CreateHighPass(float sampleRate, float cutoff, float q) {
	return BiQuadFilter(new HighPassFilter(cutoff, q, sampleRate));
}

BiQuadFilter BiQuadFilter::CreateAllPass(float sampleRate, float cutoff, float q) {
	return BiQuadFilter(new AllPassFilter(cutoff, q, sampleRate));
}
