#pragma once

#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys_devpkey.h>
#include <mmdeviceapi.h>
#include <windows.h>
#include <DPLIIVirtualSurroundDevice.h>
#include <AudioDeviceEnumerator.h>

class DPLIIVirtualSurroundConsoleApplication {
public:
    DPLIIVirtualSurroundConsoleApplication() = default;
    ~DPLIIVirtualSurroundConsoleApplication() = default;

    void Start();

private:
    void Run();
    DPLIIVirtualSurroundDevice device;
    IMMDeviceCollection* GetCompatibleDevices();
    void GetInputAndOutputDevices(IMMDevice*& inputDevice, IMMDevice*& outputDevice);
};