add_subdirectory(DPLII)
add_subdirectory(DSP)

# Create a library
add_library(VirtualSurroundApp VirtualSurroundAppV2.cpp)  # Add main app source if it's part of the lib
target_include_directories(VirtualSurroundApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include) # Include project headers
target_link_libraries(VirtualSurroundApp DPLII_lib DSP_lib) # Link with sub-libraries