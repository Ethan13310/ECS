# --- ECS Project

cmake_minimum_required(VERSION 3.8)
project(ECS CXX)

# --- C++17 Standard

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Include Directories

include_directories("include")

# --- Source Files

file(GLOB_RECURSE ECS_SOURCES "src/*.cpp")

add_library(ECS STATIC ${ECS_SOURCES})

# --- Compiler Options

if (MSVC)
    add_compile_options("/W4" "/MP")
    
    if (DEFINED ENV{TRAVIS})
        target_compile_options(ECS PRIVATE "/WX")
    endif()
else()
    add_compile_options("-Wall" "-Wextra")
    
    if (DEFINED ENV{TRAVIS})
        target_compile_options(ECS PRIVATE "-Werror")
    endif()
endif()

# --- Install Directories

install(TARGETS ECS DESTINATION "lib")
install(DIRECTORY "include/" DESTINATION "include")

# --- Unit Tests

option(WITH_UNIT_TESTS "If set, the unit tests will be built." OFF)

if (WITH_UNIT_TESTS)
    message(STATUS "Building WITH unit tests.")

    enable_testing()
    add_subdirectory("tests")
else()
    message(STATUS "Building WITHOUT unit tests. Re-run CMake with the option -DWITH_UNIT_TESTS=1 to enable them.")
endif()
