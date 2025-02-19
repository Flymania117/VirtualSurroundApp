add_library(DPLII_lib
    DPLIIVirtualSurroundConsoleApplication.cpp
    DPLIIVirtualSurroundDevice.cpp
    DPLIIVirtualSurroundDriver.cpp

    ${INCLUDE_ROOT}/DPLII/DPLIIVirtualSurroundConsoleApplication.h
    ${INCLUDE_ROOT}/DPLII/DPLIIVirtualSurroundDevice.h
    ${INCLUDE_ROOT}/DPLII/DPLIIVirtualSurroundDriver.h
)

message(STATUS ${PROJECT_SOURCE_DIR})

target_include_directories(DPLII_lib PUBLIC ${INCLUDE_ROOT}/DPLII)
target_link_libraries(DPLII_lib DSP_lib Devices_lib)