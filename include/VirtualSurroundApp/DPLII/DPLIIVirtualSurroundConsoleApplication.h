#pragma once

#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys_devpkey.h>
#include <mmdeviceapi.h>
#include <windows.h>

class DPLIIVirtualSurroundConsoleApplication {
public:
    DPLIIVirtualSurroundConsoleApplication();
    ~DPLIIVirtualSurroundConsoleApplication();

    void Start();

private:
    void Run();
    IMMDeviceCollection* GetCompatibleDevices();
    HRESULT GetInputAndOutputDevices(IMMDeviceCollection* devices, UINT deviceCount, IMMDevice*& inputDevice, IMMDevice*& outputDevice);
    HRESULT EnumerateAvailableDevices(IMMDeviceEnumerator* enumerator, IMMDeviceCollection*& devices, UINT& deviceCount);
};