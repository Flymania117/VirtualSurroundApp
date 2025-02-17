#include "Delay.h"
#include <algorithm>

Delay::Delay(int sampleRate, float delayMs) // Constructor now takes max delay
{
    _bufferSize = (int)(sampleRate * 200.0f / 1000.0f); // Maximum buffer size
    _buffer = vector<float>(_bufferSize);
    _writeIndex = 0;
    SetDelay(delayMs); // Initialize with a default delay (e.g., 10ms)
}

float Delay::Process(float x)
{
    int readIndex = (_writeIndex - _delaySamples + _bufferSize) % _bufferSize;
    float output = _buffer[readIndex];

    _buffer[_writeIndex] = x;
    _writeIndex = (_writeIndex + 1) % _bufferSize;

    return output;
}

void Delay::SetDelay(float delayMs)
{
    _delaySamples = (int)(_bufferSize * delayMs / 1000.0f); // Calculate delay samples
    _delaySamples = clamp(_delaySamples, 0, _bufferSize - 1); // Important: prevent out of range
}

float Delay::GetDelay()
{
    return _delaySamples / (_bufferSize / 1000.0f); // returns delay in ms
}