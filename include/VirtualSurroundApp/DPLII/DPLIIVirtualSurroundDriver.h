#pragma once
#include <array>

using namespace std;

class DPLIIVirtualSurroundDriver {
public:
    static constexpr float CENTER_ATTENUATION = 0.7071f;
    static constexpr float SURROUND_ATTENUATION = 0.7071f;
    static constexpr float CROSSTALK_RATIO = 0.7071f;
    static constexpr float MAX_EXAGGERATION_FACTOR = 0.2f;
    static constexpr float LFE_RATIO = 0.6f;
    static constexpr float RMS_CHANGE_RATE = 0.005f;

    DPLIIVirtualSurroundDriver() = default;
    DPLIIVirtualSurroundDriver(float sampleRate);
    ~DPLIIVirtualSurroundDriver() = default;

    void ProcessSample(float frontLeft, float frontRight, float center, float lfe, float surroundLeft, float surroundRight, float& outLeft, float& outRight);

private:
    array<float, 4> CalculateRMSForAllChannels(float frontLeft, float frontRight, float slDelayed, float srDelayed);
    void CalculateDirectionalExaggeration(float frontLeft, float frontRight, float slDelayed, float srDelayed, const array<float, 4> rmsValues, float rmsMax, float& exaggerationL, float& exaggerationR, float& exaggerationSL, float& exaggerationSR);
    void SurroundChannelCrosstalk(float left, float right, float& slCrosstalk, float& srCrosstalk);
    void FrontChannelCrosstalk(float left, float right, float& lCrosstalk, float& rCrosstalk);
    void SmoothRMSChange(array<float, 4>& targetRms);
};