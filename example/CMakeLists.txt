cmake_minimum_required (VERSION 2.6)
project (EXAMPLE)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process (COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                     OUTPUT_VARIABLE GCC_VERSION)
    string (STRIP ${GCC_VERSION} GCC_VERSION)

    if (GCC_VERSION VERSION_GREATER 4.7.0)
        set (CMAKE_CXX_FLAGS "-O2 -Wall -pedantic -std=gnu++11 -g")
    else ()
        message(FATAL_ERROR "GCC 4.7.0 or higher is required!")
    endif ()
endif ()

get_filename_component (EXAMPLE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component (EXAMPLE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)

file (GLOB_RECURSE EXAMPLE_SOURCES "src/*.cpp")
include_directories (${EXAMPLE_SOURCE_DIR}/src ${MATH_INCLUDE_DIRS})

set (EXAMPLE_EXECUTABLE "example")
add_executable (${EXAMPLE_EXECUTABLE} ${EXAMPLE_SOURCES})

link_directories (${EXAMPLE_BINARY_DIR})
target_link_libraries (${EXAMPLE_EXECUTABLE} ${GRAPHENE_SHARED})
