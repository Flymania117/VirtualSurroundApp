#pragma once

#include <vector>
#include <complex>

using namespace std;

class PhaseRotator {
public:
    PhaseRotator() = default;
    PhaseRotator(int filterLength);
    ~PhaseRotator() = default;
    float ProcessSample(float x);

private:
    vector<float> _coefficients;
    vector<float> _inputBuffer;
    int _bufferIndex;
    vector<float> GenerateHilbertFilter(int length);
};
