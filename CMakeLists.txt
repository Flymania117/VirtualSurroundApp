cmake_minimum_required(VERSION 3.20)  # Or higher, as needed
project(VirtualSurroundApp VERSION 1.0)

set(CMAKE_CXX_STANDARD 23) # Or your preferred standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INCLUDE_ROOT "${PROJECT_SOURCE_DIR}/include/VirtualSurroundApp" CACHE INTERNAL "")

# Set output directories - cleaner organization
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Include subdirectories
add_subdirectory(src)

# Targets (optional - if you have a main entry point)
# If you don't have a specific main, you can remove this
add_executable(VirtualSurroundAppMain src/VirtualSurroundAppV2.cpp) # Link against your library
target_link_libraries(VirtualSurroundAppMain VirtualSurroundApp) # Link with the library

# Install rules (optional - if you want to install)
install(TARGETS VirtualSurroundAppMain DESTINATION bin)
install(DIRECTORY include/VirtualSurroundApp DESTINATION include) # Install headers
install(FILES LICENSE README.md DESTINATION .) # Install other files