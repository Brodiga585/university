cmake_minimum_required(VERSION 3.14)
project(TuringMachineLab CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TURING_SOURCES
    src/Tape.cpp
    src/Head.cpp
    src/Rule.cpp
    src/TuringMachine.cpp
)

add_executable(turing_app src/main.cpp ${TURING_SOURCES})

enable_testing()
add_executable(turing_tests tests/test_turing.cpp ${TURING_SOURCES})