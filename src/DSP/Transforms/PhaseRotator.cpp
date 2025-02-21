#define _USE_MATH_DEFINES
#include <cmath>
#include "PhaseRotator.h"
#include <algorithm>
#include <functional>

PhaseRotator::PhaseRotator(int filterLength)
{
    _coefficients = GenerateHilbertFilter(filterLength);
    _inputBuffer = vector<float>(filterLength);
    _bufferIndex = 0;
}

vector<float> PhaseRotator::GenerateHilbertFilter(int length)
{
    auto coefficients = vector<float>(length);
    int center = length / 2;

    for (int n = 0; n < length; n++)
    {
        if (n != center)
        {
            coefficients[n] = 2.0 / M_PI * sin(M_PI * (n - center) / 2.0) / (n - center);
        }
    }

    return coefficients;
}

float PhaseRotator::ProcessSample(float x)
{
    _inputBuffer[_bufferIndex] = x;
    _bufferIndex = (_bufferIndex + 1) % _inputBuffer.size();

    float output = 0;
    for (int i = 0; i < _coefficients.size(); i++)
    {
        int index = (_bufferIndex + i) % _inputBuffer.size();
        output += _coefficients[i] * _inputBuffer[index];
    }
    return output;
}