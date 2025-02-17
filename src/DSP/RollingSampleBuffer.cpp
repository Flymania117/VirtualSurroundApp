#include "RollingSampleBuffer.h"

RollingSampleBuffer::RollingSampleBuffer(int size)
{
    _buffer = vector<float>(size);
    _index = 0;
    _count = 0;
}

float RollingSampleBuffer::Add(float value)
{
    float removed = (_count < _buffer.size()) ? 0.0f : _buffer[_index]; // Oldest sample

    _buffer[_index] = value;
    _index = (_index + 1) % _buffer.size(); // Circular buffer increment
    if (_count < _buffer.size()) _count++;

    return removed;
}

float RollingSampleBuffer::Get(int i)
{
    return _buffer[i];
}


float RollingSampleBuffer::operator[](int i)
{
    return Get(i);
}