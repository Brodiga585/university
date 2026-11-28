cmake_minimum_required(VERSION 3.14)
project(EnglishRussianDictionary CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(DICT_SOURCES
    src/Dictionary.cpp
)

add_executable(dict_app src/main.cpp ${DICT_SOURCES})

enable_testing()

add_executable(dict_tests tests/test_dictionary.cpp ${DICT_SOURCES})

target_compile_options(dict_tests PRIVATE --coverage -O0 -g)
target_link_options(dict_tests PRIVATE --coverage)

add_custom_target(coverage
    COMMAND dict_tests
    COMMAND gcovr -r ${CMAKE_CURRENT_SOURCE_DIR} --filter "${CMAKE_CURRENT_SOURCE_DIR}/src/" --exclude "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" --fail-under-line 90 --print-summary ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS dict_tests
    COMMENT "Running tests and checking coverage >= 90%..."
)