#include "AudioDevice.h"
#include <string>

AudioDevice::AudioDevice(std::string DeviceFriendlyName, UINT NumChannels, IMMDevice* device) {
	this->device = device;
	this->NumChannels = NumChannels;
	this->DeviceFriendlyName = DeviceFriendlyName;
}