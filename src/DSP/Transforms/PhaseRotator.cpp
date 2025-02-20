#define _USE_MATH_DEFINES
#include <cmath>
#include "PhaseRotator.h"
#include <complex>

using namespace std;

PhaseRotator::PhaseRotator(int filterLength)
    : filterLength_(filterLength),
    buffer_(filterLength, 0.0f),
    bufferIndex_(0)
{
    if (filterLength_ % 2 == 0) {
        filterLength_++;
        buffer_.resize(filterLength_, 0.0f);
    }
    delay_ = (filterLength_ - 1) / 2;
    coeffs_.resize(filterLength_, 0.0f);

    for (int n = 0; n < filterLength_; n++) {
        int k = n - delay_;
        float h = 0.0f;
        if (k == 0) {
            h = 0.0f;
        }
        else if (k % 2 == 0) {
            h = 0.0f;
        }
        else {
            h = 2.0f / (M_PI * k);
        }
        float w = 0.54f - 0.46f * cos(2.0f * M_PI * n / (filterLength_ - 1));
        coeffs_[n] = h * w;
    }
}

float PhaseRotator::ProcessSample(float x) {
    buffer_[bufferIndex_] = x;
    bufferIndex_ = (bufferIndex_ + 1) % filterLength_;

    float y = 0.0f;
    int index = bufferIndex_;
    for (int i = 0; i < filterLength_; i++) {
        y += coeffs_[i] * buffer_[index];
        index = (index + 1) % filterLength_;
    }
    return y;
}
