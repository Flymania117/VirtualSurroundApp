#pragma once

#include <vector>
#include <complex>

using namespace std;

class PhaseRotator {
public:
    PhaseRotator(int filterLength);
    PhaseRotator() = default;
    ~PhaseRotator() = default;

    float ProcessSample(float x);

private:
    int filterLength_;    
    int delay_;           
    vector<float> coeffs_;
    vector<float> buffer_;
    int bufferIndex_;     
};
