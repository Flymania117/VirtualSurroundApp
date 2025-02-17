add_library(Filters_lib
    AllPassFilter.cpp
    BiQuadFilter.cpp
    EQFilterCurve.cpp
    HighPassFilter.cpp
    LowPassFilter.cpp
    ${INCLUDE_ROOT}/DSP/Filters/EQFilterCurve.h
    ${INCLUDE_ROOT}/DSP/Filters/Filters.h
    ${INCLUDE_ROOT}/DSP/Filters/BiQuadFilter.h
)
target_include_directories(Filters_lib PUBLIC ${INCLUDE_ROOT}/DSP/Filters) # Include Filters headers
