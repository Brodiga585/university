cmake_minimum_required(VERSION 3.14)
project(TuringMachine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TM_SOURCES
    src/Tape.cpp
    src/Head.cpp
    src/Rule.cpp
    src/TuringMachine.cpp
)

add_executable(turing_app src/main.cpp ${TM_SOURCES})
add_executable(turing_tests tests/test_turing.cpp ${TM_SOURCES})

target_compile_options(turing_tests PRIVATE --coverage -O0 -g)
target_link_options(turing_tests PRIVATE --coverage)

add_custom_target(coverage
    COMMAND turing_tests
    COMMAND gcovr -r ${CMAKE_CURRENT_SOURCE_DIR} --filter "${CMAKE_CURRENT_SOURCE_DIR}/src/" --exclude "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" --fail-under-line 90 --print-summary ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS turing_tests
    COMMENT "Running tests and checking coverage >= 90%..."
)