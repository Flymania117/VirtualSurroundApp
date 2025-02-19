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
#include <AudioDeviceEnumerator.h>
#include <atlstr.h>

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

void DPLIIVirtualSurroundConsoleApplication::GetInputAndOutputDevices(IMMDevice*& inputDevice, IMMDevice*& outputDevice) {
	AudioDeviceEnumerator audioDeviceEnumerator;
	int inputDeviceIndex = -1;
	int outputDeviceIndex = -1;
	string message = "Please select a valid index from the list.";

	// Get input device index from user input
	print("The devices below are configured for surround and will act as the input for the application.\nSelect one of them by inputting its corresponding number:\n");
	for (UINT i = 0; i < audioDeviceEnumerator.inputAudioDevices.size(); i++) {
		print("Device [{}]: {} ({} channels)\n", i, audioDeviceEnumerator.inputAudioDevices[i].DeviceFriendlyName, audioDeviceEnumerator.inputAudioDevices[i].NumChannels);
	}

	GetInputAndContinue([&]() {
		cin >> inputDeviceIndex;
		return inputDeviceIndex < audioDeviceEnumerator.inputAudioDevices.size();
		},
		message);

	inputDevice = audioDeviceEnumerator.inputAudioDevices[inputDeviceIndex].device;

	//// Get output device index from user input
	print("The devices below are configured for stereo and will be where the application outputs to.\nSelect one of them by inputting its corresponding number:\n");
	for (UINT i = 0; i < audioDeviceEnumerator.outputAudioDevices.size(); i++) {
		print("Device [{}]: {} ({} channels)\n", i, audioDeviceEnumerator.outputAudioDevices[i].DeviceFriendlyName, audioDeviceEnumerator.outputAudioDevices[i].NumChannels);
	}

	GetInputAndContinue([&]() {
		cin >> outputDeviceIndex;
		return outputDeviceIndex < audioDeviceEnumerator.outputAudioDevices.size();
		},
		message);

	outputDevice = audioDeviceEnumerator.outputAudioDevices[outputDeviceIndex].device;
	
}

void DPLIIVirtualSurroundConsoleApplication::Start() {
	IMMDeviceEnumerator* enumerator = NULL;
	IMMDeviceCollection* devices = NULL;
	UINT deviceCount = 0;
	
	IMMDevice* inputDevice = NULL;
	IMMDevice* outputDevice = NULL;

	GetInputAndOutputDevices(inputDevice, outputDevice);

	// Run the device
	device = DPLIIVirtualSurroundDevice(inputDevice, outputDevice);
	device.Configure();
	Run();

	return;
Exit:
	printf("Application encountered an error\n");
	SAFE_RELEASE(enumerator);
	SAFE_RELEASE(devices);
	SAFE_RELEASE(inputDevice);
	SAFE_RELEASE(outputDevice);
}

void DPLIIVirtualSurroundConsoleApplication::Run() {
	device.Run();
}

IMMDeviceCollection* DPLIIVirtualSurroundConsoleApplication::GetCompatibleDevices() {
	// Gets system compatible audio devices
	return nullptr;
}