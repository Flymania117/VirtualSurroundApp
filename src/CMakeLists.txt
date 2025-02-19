add_subdirectory(DPLII)
add_subdirectory(DSP)
add_subdirectory(Devices)

add_library(VirtualSurroundApp
	VirtualSurroundAppV2.cpp
)
target_include_directories(VirtualSurroundApp PUBLIC ${INCLUDE_ROOT})
target_link_libraries(VirtualSurroundApp DPLII_lib DSP_lib Devices_lib)