#pragma once
#include <vector>

using namespace std;

class Delay {
private:
    vector<float> _buffer;
    int _writeIndex;
    int _delaySamples;
    int _bufferSize; // Keep track of the buffer size

public:
    Delay() = default;
    Delay(int sampleRate, float delayMs = 50.0f);
    ~Delay() = default;

    float Process(float x);
    void SetDelay(float delayMs);
    
    float GetDelay();
};

