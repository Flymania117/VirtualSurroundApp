#include <initguid.h>
#include "AudioDeviceEnumerator.h"
#include <Mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <vector>
#include <array>
#include <string>

#define SAFE_RELEASE(punk) if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

using namespace std;

string ConvertLPCWSTRToString(LPWSTR lpcwszStr)
{
	// Determine the length of the converted string 
	int strLength
		= WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1,
			nullptr, 0, nullptr, nullptr);

	// Create a std::string with the determined length 
	string str(strLength, 0);

	// Perform the conversion from LPCWSTR to std::string 
	WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1, &str[0],
		strLength, nullptr, nullptr);

	// Return the converted std::string 
	return str;
}

AudioDeviceEnumerator::AudioDeviceEnumerator() {
	IMMDeviceEnumerator* enumerator = NULL;
	IMMDeviceCollection* devices = NULL;
	UINT deviceCount = 0;
	IMMDevice* device = nullptr;
	LPWSTR deviceId = NULL;
	IPropertyStore* deviceProperties = nullptr;

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&enumerator);

	HRESULT hr = NULL;

	hr = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &devices);
	if (FAILED(hr)) goto Exit;

	devices->GetCount(&deviceCount);
	for (UINT i = 0; i < deviceCount; i++) {
		PROPVARIANT deviceFriendlyName;
		PROPVARIANT deviceFormat;
		PropVariantInit(&deviceFriendlyName);
		PropVariantInit(&deviceFormat);

		hr = devices->Item(i, &device);
		if (FAILED(hr)) goto Exit;

		hr = device->GetId(&deviceId);
		if (FAILED(hr)) goto Exit;

		hr = device->OpenPropertyStore(STGM_READ, &deviceProperties);
		if (FAILED(hr)) goto Exit;

		hr = deviceProperties->GetValue(PKEY_DeviceInterface_FriendlyName, &deviceFriendlyName);
		if (FAILED(hr)) goto Exit;

		hr = deviceProperties->GetValue(PKEY_AudioEngine_DeviceFormat, &deviceFormat);
		if (FAILED(hr)) goto Exit;

		WAVEFORMATEX* deviceWaveFormat = (WAVEFORMATEX*)deviceFormat.blob.pBlobData;
		
		if (static_cast<UINT>(deviceWaveFormat->nChannels) > 5)
			inputAudioDevices.push_back(AudioDevice(ConvertLPCWSTRToString(deviceFriendlyName.pwszVal), static_cast<UINT>(deviceWaveFormat->nChannels), device));

		else if(static_cast<UINT>(deviceWaveFormat->nChannels) == 2)
			outputAudioDevices.push_back(AudioDevice(ConvertLPCWSTRToString(deviceFriendlyName.pwszVal), 2, device));

		CoTaskMemFree(deviceId);
		deviceId = NULL;
		hr = PropVariantClear(&deviceFriendlyName);
		hr = PropVariantClear(&deviceFormat);
		if (FAILED(hr)) goto Exit;

		SAFE_RELEASE(deviceProperties);
		SAFE_RELEASE(device);
	}

Exit:
	CoTaskMemFree(deviceId);
	SAFE_RELEASE(enumerator);
	SAFE_RELEASE(device);
	SAFE_RELEASE(deviceProperties);
}

void AudioDeviceEnumerator::EnumerateAvailableDevices(Flow flow, vector<AudioDevice>& devices) {
	if (flow == INPUT)
		devices = inputAudioDevices;
	else
		devices = outputAudioDevices;
}