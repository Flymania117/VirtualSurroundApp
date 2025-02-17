#pragma once
#include "RollingSampleBuffer.h"

class RMSCalculator {
private:
    RollingSampleBuffer _buffer;
    int _rmsWindowSize;
    float _sumOfSquares;

public:
    RMSCalculator() = default;
    RMSCalculator(int sampleRate, int rmsWindowMilliseconds);
    ~RMSCalculator() = default;
    float GetCurrentRMS(float input);
};
