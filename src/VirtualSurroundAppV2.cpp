#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys_devpkey.h>
#include <iostream>
#include "DPLIIVirtualSurroundConsoleApplication.h"

#define REFTIMES_PER_SEC  10000000
#define SAFE_RELEASE(punk) if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

using namespace std;

int main() {
	CoInitialize(NULL);

	DPLIIVirtualSurroundConsoleApplication application;
	application.Start();
}