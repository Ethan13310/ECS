cmake_minimum_required(VERSION 3.9)

project("ECS" CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.1")
	# G++ minimal version (7.1)
	message(FATAL_ERROR "GCC version 7.1 or greater is required.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0")
	# Clang minimal version (5.0)
	message(FATAL_ERROR "Clang version 5.0 or greater is required.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.1")
	# MSVC minimal version (14.1)
	message(FATAL_ERROR "Microsoft Visual C++ version 2017 (14.1) or greater is required.")
endif ()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CONFIGURATION_TYPES Release Debug)

if (MSVC)
	# Multicore build with Visual Studio
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

include_directories("${CMAKE_SOURCE_DIR}/include")

file(GLOB_RECURSE ECS_HEADERS "${CMAKE_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE ECS_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_library("ECS" "${ECS_SOURCES}" "${ECS_HEADERS}")

if (NOT WIN32)
	# Install directories
	install(TARGETS "ECS" ARCHIVE DESTINATION "lib")
	install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION "include")
endif ()
