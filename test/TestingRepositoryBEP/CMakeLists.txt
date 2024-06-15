cmake_minimum_required(VERSION 3.28)
project(TestingRepositoryBEP)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(lander_communication_run ${SOURCE_FILES})

include_directories(lander_communication_lib)
add_subdirectory(lander_communication_lib)

target_link_libraries(lander_communication_run lander_communication_lib)

add_subdirectory(Google_tests)
