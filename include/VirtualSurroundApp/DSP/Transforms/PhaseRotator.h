#pragma once
#include <vector>
#include <corecrt_math.h>
#include <corecrt_math_defines.h>

using namespace std;

class PhaseRotator {
public:
    PhaseRotator() = default;
    PhaseRotator(int filterLength);
    ~PhaseRotator() = default;
    float ProcessSamplePositive(float x);
    float ProcessSampleNegative(float x);

private:
    vector<float> _positiveFilterCoefficients;
    vector<float> _negativeFilterCoefficients;
    vector<float> _inputBuffer;
    int _bufferIndex;
    vector<float> GenerateHilbertFilter(int length);
    vector<float> GenerateNegativeHilbertFilter(int length);
    float FilterSample(float x, const vector<float>& coefficients);
};

