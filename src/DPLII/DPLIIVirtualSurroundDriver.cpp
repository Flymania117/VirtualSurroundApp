#include "DPLIIVirtualSurroundDriver.h"
#include "BiQuadFilter.h"
#include "Delay.h"
#include "RMSCalculator.h"
#include "PhaseRotator.h"
#include <algorithm>

using namespace std;

// Base DSP Components
BiQuadFilter _lfeFilter;

BiQuadFilter _surroundCrossoverL;
BiQuadFilter _surroundCrossoverR;

PhaseRotator _phaseRotatorL;
PhaseRotator _phaseRotatorR;

Delay _surroundDelayL;
Delay _surroundDelayR;
Delay _surroundDelayLFE;

// DSP Components for enhancements
BiQuadFilter _crosstalkAllPassL;
BiQuadFilter _crosstalkAllPassR;
BiQuadFilter _crosstalkAllPassSL;
BiQuadFilter _crosstalkAllPassSR;
BiQuadFilter _crosstalkFilterL;
BiQuadFilter _crosstalkFilterR;
BiQuadFilter _crosstalkFilterSL;
BiQuadFilter _crosstalkFilterSR;

Delay _crosstalkDelayL;
Delay _crosstalkDelayR;
Delay _crosstalkDelaySL;
Delay _crosstalkDelaySR;

RMSCalculator _rmsL;
RMSCalculator _rmsR;
RMSCalculator _rmsSL;
RMSCalculator _rmsSR;

array<float, 4> currentRmsValues { -1.0f, -1.0f, -1.0f, -1.0f };

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

static float SoftClip(float input, float threshold = 0.9f)
{
    if (abs(input) <= threshold)
        return input; // Pass through if within threshold

    // Apply soft clipping beyond the threshold
    return (float)(sign(input) * (threshold + (1 - threshold) * tanh((abs(input) - threshold) / (1 - threshold))));
}

static float LinearToExponential(float input)
{
    return (pow(M_E, 2 * input) - 1) / (pow(M_E, 2) - 1);
}

DPLIIVirtualSurroundDriver::DPLIIVirtualSurroundDriver(float sampleRate) {
    // BASE DPL II DSP

    // low-pass @ 120Hz for LFE
    _lfeFilter = BiQuadFilter::CreateLowPass(sampleRate, 120);

    // highpass @ 100Hz for surrounds
    _surroundCrossoverL = BiQuadFilter::CreateHighPass(sampleRate, 50);
    _surroundCrossoverR = BiQuadFilter::CreateHighPass(sampleRate, 50);

    // 90 degree phase rotator
    _phaseRotatorL = PhaseRotator(255);
    _phaseRotatorR = PhaseRotator(255);

    // Delay for surrounds
    _surroundDelayL = Delay((int)sampleRate, 10.0f);
    _surroundDelayR = Delay((int)sampleRate, 10.0f);


    // ENHANCEMENTS

    // RMS calculators for focal point exaggeration
    _rmsL = RMSCalculator((int)sampleRate, 50);
    _rmsR = RMSCalculator((int)sampleRate, 50);
    _rmsSL = RMSCalculator((int)sampleRate, 50);
    _rmsSR = RMSCalculator((int)sampleRate, 50);

    // Crosstalk filters
    _crosstalkAllPassL = BiQuadFilter::CreateAllPass(sampleRate, 500);
    _crosstalkAllPassR = BiQuadFilter::CreateAllPass(sampleRate, 500);
    _crosstalkAllPassSL = BiQuadFilter::CreateAllPass(sampleRate, 500);
    _crosstalkAllPassSR = BiQuadFilter::CreateAllPass(sampleRate, 500);
    _crosstalkFilterL = BiQuadFilter::CreateLowPass(sampleRate, 4000);
    _crosstalkFilterR = BiQuadFilter::CreateLowPass(sampleRate, 4000);
    _crosstalkFilterSL = BiQuadFilter::CreateLowPass(sampleRate, 8000);
    _crosstalkFilterSR = BiQuadFilter::CreateLowPass(sampleRate, 8000);


    // 1ms delay for crosstalk enhancement
    _crosstalkDelayL = Delay((int)sampleRate, 1.0f);
    _crosstalkDelayR = Delay((int)sampleRate, 1.0f);
    _crosstalkDelaySL = Delay((int)sampleRate, 1.0f);
    _crosstalkDelaySR = Delay((int)sampleRate, 1.0f);

    // LFE delay for spill into surround
    _surroundDelayLFE = Delay((int)sampleRate, 20.0f);
}

void DPLIIVirtualSurroundDriver::ProcessSample(float frontLeft, float frontRight, float center, float lfe, float surroundLeft, float surroundRight, float& outLeft, float& outRight)
{
    // Process LFE channel
    auto lfeOut = _lfeFilter.Process(lfe) * LFE_RATIO;

    // Process center channel
    center *= CENTER_ATTENUATION;

    frontLeft *= 0.7071f;
    frontRight *= 0.7071f;

    // Process surround channels
    auto sLfe = _surroundDelayLFE.Process(lfeOut);
    auto slBandpassed = _surroundCrossoverL.Process(surroundLeft) + sLfe;
    auto srBandpassed = _surroundCrossoverR.Process(surroundRight) + sLfe;

    // Apply 90° phase shifts and attenuation
    auto slRotated = -_phaseRotatorL.ProcessSamplePositive(slBandpassed) * SURROUND_MATCH_ATTENUATION; // Positive 90 phase shift
    auto srRotated = _phaseRotatorR.ProcessSamplePositive(srBandpassed) * SURROUND_OPPOSITE_ATTENUATION; // Positive 90 phase shift


    // Apply delay to surrounds
    auto slDelayed = _surroundDelayL.Process(slRotated);
    auto srDelayed = _surroundDelayR.Process(srRotated);

    // Experiments
    array<float, 4> rmsValues = CalculateRMSForAllChannels(frontLeft, frontRight, slDelayed, srDelayed);
    float rmsMax;
    float rmsMin;

    if (currentRmsValues[0] == -1.0f)
        currentRmsValues = rmsValues;

    rmsMax = *max_element(rmsValues.begin(), rmsValues.end());
    rmsMin = *min_element(rmsValues.begin(), rmsValues.end());

    for (int i = 0; i < 4 && rmsMax > 0.0f; i++)
    {
        rmsValues[i] = clamp(LinearToExponential((rmsValues[i] - rmsMin) / (rmsMax - rmsMin)), 0.1f, 1.0f);
    }
    

    SmoothRMSChange(rmsValues);

    // Focal exageration
    float exaggerationL = 0.0f, exaggerationR = 0.0f, exaggerationSL = 0.0f, exaggerationSR = 0.0f;

    CalculateDirectionalExaggeration(frontLeft, frontRight, slDelayed, srDelayed, rmsValues, rmsMax, exaggerationL, exaggerationR, exaggerationSL, exaggerationSR);


    // Additional crosstalk between left and right sides

    float lCrosstalk = 0.0f, rCrosstalk = 0.0f;
    float slCrosstalk = 0.0f, srCrosstalk = 0.0f;

    // Front channels
    FrontChannelCrosstalk(frontLeft, frontRight, lCrosstalk, rCrosstalk);

    // Surround channels
    SurroundChannelCrosstalk(slDelayed, srDelayed, slCrosstalk, srCrosstalk);

    // RMS dependent crosstalk intensity
    auto lCrossTalkRmsFactor = rmsMax == 0 ? 0 : lCrosstalk * currentRmsValues[0];
    auto slCrossTalkRmsFactor = rmsMax == 0 ? 0 : slCrosstalk * currentRmsValues[2];
    auto rCrossTalkRmsFactor = rmsMax == 0 ? 0 : rCrosstalk * currentRmsValues[1];
    auto srCrossTalkRmsFactor = rmsMax == 0 ? 0 : srCrosstalk * currentRmsValues[3];


    // Matrix mixing (Dolby Pro Logic II)
    auto lt = frontLeft + exaggerationL +
        center + lfeOut +
        slDelayed + exaggerationSL +
        rCrossTalkRmsFactor + srCrossTalkRmsFactor;

    auto rt = frontRight + exaggerationR +
        center + lfeOut +
        srDelayed + exaggerationSR +
        lCrossTalkRmsFactor + slCrossTalkRmsFactor;

    // Prevent clipping
    outLeft = SoftClip(lt);
    outRight = SoftClip(rt);
}

void DPLIIVirtualSurroundDriver::CalculateDirectionalExaggeration(float frontLeft, float frontRight, float slDelayed, float srDelayed, const array<float, 4> rmsValues, float rmsMax, float& exaggerationL, float& exaggerationR, float& exaggerationSL, float& exaggerationSR)
{
    if (rmsMax == 0)
    {
        exaggerationL = 0;
        exaggerationR = 0;
        exaggerationSL = 0;
        exaggerationSR = 0;
        return;
    }

    exaggerationL = SoftClip(frontLeft * MAX_EXAGGERATION_FACTOR * currentRmsValues[0]);
    exaggerationR = SoftClip(frontRight * MAX_EXAGGERATION_FACTOR * currentRmsValues[1]);
    exaggerationSL = SoftClip((abs(slDelayed) < 0.00001 ? 0 : slDelayed) * MAX_EXAGGERATION_FACTOR * currentRmsValues[2]);
    exaggerationSR = SoftClip((abs(srDelayed) < 0.00001 ? 0 : srDelayed) * MAX_EXAGGERATION_FACTOR * currentRmsValues[3]);
}

array<float,4> DPLIIVirtualSurroundDriver::CalculateRMSForAllChannels(float frontLeft, float frontRight, float slDelayed, float srDelayed)
{
    array<float, 4> rmsValues;
    rmsValues[0] = abs(_rmsL.GetCurrentRMS(frontLeft));
    rmsValues[1] = abs(_rmsR.GetCurrentRMS(frontRight));
    rmsValues[2] = abs(_rmsSL.GetCurrentRMS(slDelayed));
    rmsValues[3] = abs(_rmsSR.GetCurrentRMS(srDelayed));
    return rmsValues;
}

void DPLIIVirtualSurroundDriver::SurroundChannelCrosstalk(float left, float right, float& slCrosstalk, float& srCrosstalk)
{
    auto slCrosstalkAP = _crosstalkAllPassSL.Process(left);
    auto slCrosstalkLP = _crosstalkFilterSL.Process(slCrosstalkAP);
    slCrosstalk = _crosstalkDelaySL.Process(slCrosstalkLP) * CROSSTALK_ATTENUATION;

    auto srCrosstalkAP = _crosstalkAllPassSR.Process(right);
    auto srCrosstalkLP = _crosstalkFilterSR.Process(srCrosstalkAP);
    srCrosstalk = _crosstalkDelaySR.Process(srCrosstalkLP) * CROSSTALK_ATTENUATION;
}

void DPLIIVirtualSurroundDriver::FrontChannelCrosstalk(float left, float right, float& lCrosstalk, float& rCrosstalk)
{
    auto lCrosstalkAP = _crosstalkAllPassL.Process(left);
    auto lCrosstalkLP = _crosstalkFilterL.Process(lCrosstalkAP);
    lCrosstalk = _crosstalkDelayL.Process(lCrosstalkLP) * CROSSTALK_ATTENUATION;

    auto rCrosstalkAP = _crosstalkAllPassR.Process(right);
    auto rCrosstalkLP = _crosstalkFilterR.Process(rCrosstalkAP);
    rCrosstalk = _crosstalkDelayR.Process(rCrosstalkLP) * CROSSTALK_ATTENUATION;
}

void DPLIIVirtualSurroundDriver::SmoothRMSChange(array<float, 4>& targetRms)
{
    for (int i = 0; i < 4; i++)
    {
        currentRmsValues[i] = clamp(currentRmsValues[i] + RMS_CHANGE_RATE * (targetRms[i] - currentRmsValues[i]), 0.0f, 1.0f);
    }
}