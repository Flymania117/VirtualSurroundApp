add_library(Devices_lib
    AudioDevice.cpp
    AudioDeviceEnumerator.cpp
    ${INCLUDE_ROOT}/Devices/AudioDevice.h
    ${INCLUDE_ROOT}/Devices/AudioDeviceEnumerator.h
)
    
target_include_directories(Devices_lib PUBLIC ${INCLUDE_ROOT}/Devices)