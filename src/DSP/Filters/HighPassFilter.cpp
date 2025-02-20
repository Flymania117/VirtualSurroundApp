#define _USE_MATH_DEFINES
#include <cmath>
#include "Filters.h"

using namespace std;

HighPassFilter::HighPassFilter(float frequency, float Q, uint32_t sampleRate) {
	SetCoefficients(frequency, Q, 0, sampleRate);
}

void HighPassFilter::SetCoefficients(float frequency, float Q, float gain, uint32_t sampleRate){
    float w0 = 2.0f * M_PI * (frequency / static_cast<float>(sampleRate));
    float sinW0 = sin(w0);
    float cosW0 = cos(w0);
    float alpha = sinW0 / (2.0f * Q);


    _bCoefficients[0] = (1.0f + cosW0) / 2.0f;
    _bCoefficients[1] = -(1.0f + cosW0);
    _bCoefficients[2] = (1.0f + cosW0) / 2.0f;

    _aCoefficients[0] = 1.0f + alpha;
    _aCoefficients[1] = -2.0f * cosW0;
    _aCoefficients[2] = 1.0f - alpha;
}