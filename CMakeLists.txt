cmake_minimum_required(VERSION 3.9)

project("ECS" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.1")
	message(FATAL_ERROR "GCC version 7.1 or greater is required.")
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

include_directories("include")

file(GLOB_RECURSE ECS_HEADERS "include/*.hpp")
file(GLOB_RECURSE ECS_SOURCES "src/*.cpp")

add_library("ECS" ${ECS_SOURCES} ${ECS_HEADERS})

if (WIN32)
	install(TARGETS "ECS" DESTINATION ".")
else ()
	install(TARGETS "ECS" DESTINATION "lib")
endif ()
