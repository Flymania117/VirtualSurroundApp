add_library(Transforms_lib
    Delay.cpp
    PhaseRotator.cpp
    ${INCLUDE_ROOT}/DSP/Transforms/Delay.h
    ${INCLUDE_ROOT}/DSP/Transforms/PhaseRotator.h
)
target_include_directories(Transforms_lib PUBLIC ${INCLUDE_ROOT}/DSP/Transforms) # Include Transforms headers

