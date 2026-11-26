cmake_minimum_required(VERSION 3.14)
project(PizzeriaLab CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PIZZERIA_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

file(GLOB_RECURSE PIZZERIA_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER PIZZERIA_SOURCES EXCLUDE REGEX "/src/main\\.cpp$")

add_library(pizzeria_core STATIC ${PIZZERIA_SOURCES})
target_include_directories(pizzeria_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(pizzeria_app src/main.cpp)
target_link_libraries(pizzeria_app PRIVATE pizzeria_core)

if(MSVC)
    target_compile_options(pizzeria_core PRIVATE /W4 /permissive-)
    target_compile_options(pizzeria_app PRIVATE /W4 /permissive-)
    if(PIZZERIA_WARNINGS_AS_ERRORS)
        target_compile_options(pizzeria_core PRIVATE /WX)
        target_compile_options(pizzeria_app PRIVATE /WX)
    endif()
else()
    target_compile_options(pizzeria_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(pizzeria_app PRIVATE -Wall -Wextra -Wpedantic)
    if(PIZZERIA_WARNINGS_AS_ERRORS)
        target_compile_options(pizzeria_core PRIVATE -Werror)
        target_compile_options(pizzeria_app PRIVATE -Werror)
    endif()
endif()

set_target_properties(pizzeria_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
