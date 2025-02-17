#include "DPLIIVirtualSurroundDevice.h"
#include "BiQuadFilter.h"
#include "DPLIIVirtualSurroundDriver.h"

#define REFTIMES_PER_SEC  10000000
#define SAFE_RELEASE(punk) if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

using namespace std;

DPLIIVirtualSurroundDriver driver;

static void Downmix6ChToStereo(float* input, float* output, UINT32 frameCount) {
	for (UINT32 i = 0; i < frameCount; i++) {
		float* in = &input[i * 6];
		float* out = &output[i * 2];

		driver.ProcessSample(in[0], in[1], in[2], in[3], in[4], in[5], out[0], out[1]);
		continue;
	}
}

DPLIIVirtualSurroundDevice::~DPLIIVirtualSurroundDevice() { }

DPLIIVirtualSurroundDevice::DPLIIVirtualSurroundDevice(IMMDevice* inputDevice, IMMDevice* outputDevice) {
	this->inputDevice = inputDevice;
	this->outputDevice = outputDevice;
}

HRESULT DPLIIVirtualSurroundDevice::Configure() {
	HRESULT hr = NULL;

	hr = inputDevice->Activate(__uuidof(IAudioClient),
		CLSCTX_ALL, NULL, (void**)&inputClient);

	hr = inputClient->GetMixFormat(&inputFormat);

	hr = inputClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
		AUDCLNT_STREAMFLAGS_LOOPBACK, REFTIMES_PER_SEC, 0, inputFormat, NULL);

	hr = inputClient->GetService(__uuidof(IAudioCaptureClient), (void**)&inputInterface);

	hr = outputDevice->Activate(__uuidof(IAudioClient),
		CLSCTX_ALL, NULL, (void**)&outputClient);

	hr = outputClient->GetMixFormat(&outputFormat);

	hr = outputClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
		0, REFTIMES_PER_SEC, 0, outputFormat, NULL);

	hr = outputClient->GetService(__uuidof(IAudioRenderClient), (void**)&outputInterface);

	driver = DPLIIVirtualSurroundDriver(inputFormat->nSamplesPerSec);

	return hr;
}

HRESULT DPLIIVirtualSurroundDevice::Run() {
	HRESULT hr = NULL;

	hr = inputClient->Start();
	hr = outputClient->Start();

	while (true) {
		// Check for captured data
		UINT32 packetLength;
		hr = inputInterface->GetNextPacketSize(&packetLength);
		if (FAILED(hr)) break;

		if (packetLength > 0) {
			BYTE* inputBuffer;
			UINT32 numFrames;
			DWORD flags;

			hr = inputInterface->GetBuffer(&inputBuffer, &numFrames, &flags, NULL, NULL);
			if (FAILED(hr)) break;

			// Process data
			float* input = (float*)inputBuffer;
			UINT32 outputSize = numFrames * 2 * sizeof(float);
			float* output = new float[numFrames * 2];
			Downmix6ChToStereo(input, output, numFrames);

			// Write to playback buffer
			BYTE* outputBuffer;
			UINT32 padding;
			outputClient->GetCurrentPadding(&padding);
			UINT32 availableFrames = 0;
			outputClient->GetBufferSize(&availableFrames);
			availableFrames -= padding;

			if (availableFrames >= numFrames) {
				hr = outputInterface->GetBuffer(numFrames, &outputBuffer);
				if (SUCCEEDED(hr)) {
					memcpy(outputBuffer, output, outputSize);
					outputInterface->ReleaseBuffer(numFrames, 0);
				}
			}

			delete[] output;
			inputInterface->ReleaseBuffer(numFrames);
		}

		//// Check for user input
		//if (GetAsyncKeyState(VK_RETURN) & 0x8000) break;
		//Sleep(10);
	}
	return hr;
}