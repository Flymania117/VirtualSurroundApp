#include "PhaseRotator.h"
#include <algorithm>
#include <functional>

PhaseRotator::PhaseRotator(int filterLength)
{
    _positiveFilterCoefficients = GenerateHilbertFilter(filterLength);
    _negativeFilterCoefficients = GenerateNegativeHilbertFilter(filterLength); // New: Negative filter
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

vector<float> PhaseRotator::GenerateNegativeHilbertFilter(int length)
{
    auto coefficients = GenerateHilbertFilter(length);
    vector<float> invertedCoefficients(length);
    transform(coefficients.begin(), coefficients.end(), invertedCoefficients.begin(), negate<float>());
    return invertedCoefficients;
}

float PhaseRotator::ProcessSamplePositive(float x)
{
    return (float)FilterSample(x, _positiveFilterCoefficients);
}

float PhaseRotator::ProcessSampleNegative(float x)
{
    return (float)FilterSample(x, _negativeFilterCoefficients);
}


float PhaseRotator::FilterSample(float x, const vector<float>& coefficients)
{
    _inputBuffer[_bufferIndex] = x;
    _bufferIndex = (_bufferIndex + 1) % _inputBuffer.size();

    float output = 0;
    for (int i = 0; i < coefficients.size(); i++)
    {
        int index = (_bufferIndex + i) % _inputBuffer.size();
        output += coefficients[i] * _inputBuffer[index];
    }
    return output;
}