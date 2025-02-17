#include "RMSCalculator.h"

using namespace std;

RMSCalculator::RMSCalculator(int sampleRate, int rmsWindowMilliseconds)
{
    _rmsWindowSize = (int)(sampleRate * (rmsWindowMilliseconds / 1000.0f));
    _buffer = RollingSampleBuffer(_rmsWindowSize);
}

float RMSCalculator::GetCurrentRMS(float input)
{
    float oldestSample = _buffer.Add(input);

    _sumOfSquares += (input * input); // Maintain running sum

    // Ensure _sumOfSquares does not become negative due to precision errors
    _sumOfSquares = max(0.0f, _sumOfSquares - (oldestSample * oldestSample));

    if (_rmsWindowSize <= 0)
        return -INFINITY;  // Prevent division by zero

    float rms = sqrt(_sumOfSquares / _rmsWindowSize);

    return rms;
}