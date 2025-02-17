#include <print>
#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys_devpkey.h>
#include "DPLIIVirtualSurroundConsoleApplication.h"
#include <functional>
#include "DPLIIVirtualSurroundDevice.h"

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

using namespace std;

static void GetInputAndContinue(function<bool()> canContinueDelegate, string& message) {
	while (!canContinueDelegate()) {
		print("{}\n", message);
	}
}

DPLIIVirtualSurroundConsoleApplication::DPLIIVirtualSurroundConsoleApplication() {

}

DPLIIVirtualSurroundConsoleApplication::~DPLIIVirtualSurroundConsoleApplication() {

}

HRESULT DPLIIVirtualSurroundConsoleApplication::GetInputAndOutputDevices(IMMDeviceCollection* devices, UINT deviceCount, IMMDevice*& inputDevice, IMMDevice*& outputDevice) {
	int inputDeviceIndex = -1;
	int outputDeviceIndex = -1;
	string message = "Please select a valid index from the list.";
	HRESULT hr = NULL;

	// Get input device index from user input
	printf("Select one of the above drivers by inputing its corresponding number:\n");
	GetInputAndContinue([&]() {
		cin >> inputDeviceIndex;
		return inputDeviceIndex < deviceCount;
		},
		message);
	hr = devices->Item(inputDeviceIndex, &inputDevice);
	if (FAILED(hr)) return hr;

	// Get output device index from user input
	printf("Select one of the above drivers by inputing its corresponding number:\n");
	GetInputAndContinue([&]() {
		cin >> outputDeviceIndex;
		message = outputDeviceIndex == inputDeviceIndex ? "Input and output device cannot be the same." : message;
		return outputDeviceIndex < deviceCount && outputDeviceIndex != inputDeviceIndex;
		},
		message);
	hr = devices->Item(outputDeviceIndex, &outputDevice);
	
	return hr;
}

HRESULT DPLIIVirtualSurroundConsoleApplication::EnumerateAvailableDevices(IMMDeviceEnumerator* enumerator, IMMDeviceCollection*& devices, UINT& deviceCount) {
	IMMDevice* device = nullptr;
	LPWSTR deviceId = NULL;
	IPropertyStore* deviceProperties = nullptr;
	
	HRESULT hr = NULL;

	hr = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &devices);
	if (FAILED(hr)) goto Exit;

	devices->GetCount(&deviceCount);
	for (UINT i = 0; i < deviceCount; i++) {
		PROPVARIANT deviceFriendlyName;
		PropVariantInit(&deviceFriendlyName);

		hr = devices->Item(i, &device);
		if (FAILED(hr)) goto Exit;

		hr = device->GetId(&deviceId);
		if (FAILED(hr)) goto Exit;

		hr = device->OpenPropertyStore(STGM_READ, &deviceProperties);
		if (FAILED(hr)) goto Exit;

		hr = deviceProperties->GetValue(PKEY_DeviceInterface_FriendlyName, &deviceFriendlyName);
		if (FAILED(hr)) goto Exit;

		printf("Device %d: %S\n",
			i, deviceFriendlyName.pwszVal);

		CoTaskMemFree(deviceId);
		deviceId = NULL;
		hr = PropVariantClear(&deviceFriendlyName);
		if (FAILED(hr)) return hr;

		SAFE_RELEASE(deviceProperties);
		SAFE_RELEASE(device);
	}

	return hr;
Exit:
	CoTaskMemFree(deviceId);
	SAFE_RELEASE(device);
	SAFE_RELEASE(deviceProperties);
	return hr;
}

void DPLIIVirtualSurroundConsoleApplication::Start() {
	IMMDeviceEnumerator* enumerator = NULL;
	IMMDeviceCollection* devices = NULL;
	UINT deviceCount = 0;
	
	IMMDevice* inputDevice = NULL;
	IMMDevice* outputDevice = NULL;

	DPLIIVirtualSurroundDevice device;

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&enumerator);

	if(FAILED(EnumerateAvailableDevices(enumerator, devices, deviceCount))) goto Exit;


	if(FAILED(GetInputAndOutputDevices(devices, deviceCount, inputDevice, outputDevice))) goto Exit;

	// Run the device
	device = DPLIIVirtualSurroundDevice(inputDevice, outputDevice);
	device.Configure();
	device.Run();

	return;
Exit:
	printf("Application encountered an error\n");
	SAFE_RELEASE(enumerator);
	SAFE_RELEASE(devices);
	SAFE_RELEASE(inputDevice);
	SAFE_RELEASE(outputDevice);
}

void DPLIIVirtualSurroundConsoleApplication::Run() {
	// Configure and run device
	// Get input for special instructions
}

IMMDeviceCollection* DPLIIVirtualSurroundConsoleApplication::GetCompatibleDevices() {
	// Gets system compatible audio devices
	return nullptr;
}