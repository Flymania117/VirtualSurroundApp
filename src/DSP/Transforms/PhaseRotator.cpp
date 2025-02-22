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
        int k = n - center;
        if (k == 0)
        {
            coefficients[n] = 0.0f;  // Hilbert Transform has 0 at center
        }
        else if (k % 2 != 0)  // Only compute for odd indices
        {
            coefficients[n] = (2.0f / (M_PI * k)) * sin(M_PI * k / 2.0);
        }
        else
        {
            coefficients[n] = 0.0f;  // Zero for even indices
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