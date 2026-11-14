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
add_executable(dict_tests tests/test_dictionaty.cpp ${DICT_SOURCES})