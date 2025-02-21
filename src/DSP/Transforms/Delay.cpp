#include "Delay.h"
#include <algorithm>
#include <array>

Delay::Delay(int sampleRate, float delayMs) 
{
    _bufferSize = (int)(sampleRate * delayMs / 1000.0f) + 1;
    _buffer = vector<float>(_bufferSize, 0.0f);
    _writeIndex = _bufferSize - 1;
    _readIndex = 0;
}

float Delay::Process(float x)
{
    float output = _buffer[_readIndex++];
    _readIndex %= _bufferSize;

    _buffer[_writeIndex++] = x;
    _writeIndex %= _bufferSize;

    return output;
}


