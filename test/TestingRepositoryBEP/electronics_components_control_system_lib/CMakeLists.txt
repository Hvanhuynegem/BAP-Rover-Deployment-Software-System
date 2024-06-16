project(electronics)

set(HEADER_FILES
        supercap_readout.h
        temp_sensors.h
)

set(SOURCE_FILES
        supercap_readout.cpp
        temp_sensors.cpp
)

add_library(electronics_components_control_system_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})