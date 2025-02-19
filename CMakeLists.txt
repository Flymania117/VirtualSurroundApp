cmake_minimum_required(VERSION 3.20)
project(VirtualSurroundApp VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INCLUDE_ROOT "${PROJECT_SOURCE_DIR}/include/VirtualSurroundApp" CACHE INTERNAL "")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)

add_executable(VirtualSurroundAppMain src/VirtualSurroundAppV2.cpp)
target_link_libraries(VirtualSurroundAppMain VirtualSurroundApp)

install(TARGETS VirtualSurroundAppMain DESTINATION bin)
install(DIRECTORY include/VirtualSurroundApp DESTINATION include)
install(FILES LICENSE README.md DESTINATION .)