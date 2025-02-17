add_subdirectory(Filters)
add_subdirectory(Transforms)

add_library(DSP_lib
    RMSCalculator.cpp
    RollingSampleBuffer.cpp
    ${INCLUDE_ROOT}/DSP/RollingSampleBuffer.h
    ${INCLUDE_ROOT}/DSP/RMSCalculator.h
)
target_include_directories(DSP_lib PUBLIC ${INCLUDE_ROOT}/DSP) # Include DSP headers
target_link_libraries(DSP_lib Filters_lib Transforms_lib)