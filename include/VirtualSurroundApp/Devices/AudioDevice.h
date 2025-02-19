#pragma once
#include <wtypes.h>
#include <mmdeviceapi.h>
#include <string>

struct AudioDevice {
public:
	std::string DeviceFriendlyName;
	UINT NumChannels;
	IMMDevice* device;
	AudioDevice(std::string DeviceFriendlyName, UINT NumChannels, IMMDevice* device);
	AudioDevice() = default;
	~AudioDevice() = default;
};