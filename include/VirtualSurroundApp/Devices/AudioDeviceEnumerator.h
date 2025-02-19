#pragma once
#include <mmdeviceapi.h>
#include <AudioDevice.h>
#include <vector>

using namespace std;

class AudioDeviceEnumerator {
public:
	enum Flow {
		INPUT,
		OUTPUT
	};
	vector<AudioDevice> inputAudioDevices;
	vector<AudioDevice> outputAudioDevices;
	AudioDeviceEnumerator();
	~AudioDeviceEnumerator() = default;
	void EnumerateAvailableDevices(Flow flow, vector<AudioDevice>& devices);

};