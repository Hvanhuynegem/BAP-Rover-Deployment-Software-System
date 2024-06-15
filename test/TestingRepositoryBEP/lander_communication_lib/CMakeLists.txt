project(landerCommunication)

set(HEADER_FILES
        lander_communication.h
        lander_communication_protocol.h
#        uart_communication.h
)

set(SOURCE_FILES
        lander_communication.cpp
        lander_communication_protocol.cpp
#        uart_communication.cpp
)

add_library(lander_communication_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})