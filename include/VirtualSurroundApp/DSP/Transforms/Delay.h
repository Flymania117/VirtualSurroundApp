#pragma once
#include <vector>

using namespace std;

class Delay {
private:
    vector<float> _buffer;
    int _writeIndex;
    int _readIndex;
    int _bufferSize;

public:
    Delay() = default;
    Delay(int sampleRate, float delayMs = 50.0f);
    ~Delay() = default;

    float Process(float x);
};

