#pragma once

#include <mmdeviceapi.h>
#include <Audioclient.h>

class DPLIIVirtualSurroundDevice {
public:
	DPLIIVirtualSurroundDevice() = default;
	DPLIIVirtualSurroundDevice(IMMDevice* inputDevice, IMMDevice* outputDevice);
	~DPLIIVirtualSurroundDevice();

	HRESULT Configure();
	HRESULT Run();

private:
	IMMDevice* inputDevice;
	IMMDevice* outputDevice;

	IAudioClient* inputClient;
	IAudioClient* outputClient;

	IAudioCaptureClient* inputInterface;
	IAudioRenderClient* outputInterface;

	WAVEFORMATEX* inputFormat;
	WAVEFORMATEX* outputFormat;
};