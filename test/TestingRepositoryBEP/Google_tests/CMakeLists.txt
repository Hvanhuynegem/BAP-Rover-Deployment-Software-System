project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(Google_Tests_run all_tests.cpp
        temp_sensors_test.cpp
        supercap_readout_tests.cpp)

#slip_decoding_tests.cpp slip_encoding_tests.cpp
#        convert_array_to_message_tests.cpp convert_message_to_array_tests.cpp
#        send_message_struct_tests.cpp send_message_struct_tests.cpp
#        all_tests.cpp
target_link_libraries(Google_Tests_run lander_communication_lib)

target_link_libraries(Google_Tests_run electronics_components_control_system_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)