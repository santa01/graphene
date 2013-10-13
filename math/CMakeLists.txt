cmake_minimum_required (VERSION 2.8.9)
project (MATH)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process (COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                     OUTPUT_VARIABLE GCC_VERSION)
    string (STRIP ${GCC_VERSION} GCC_VERSION)

    if (GCC_VERSION VERSION_GREATER 4.4.0)
        set (CMAKE_CXX_FLAGS "-O2 -Wall -pedantic")
    else ()
        message(FATAL_ERROR "GCC 4.4.0 or higher is required!")
    endif ()
endif ()

find_package (Doxygen)

file (GLOB_RECURSE MATH_SOURCES "src/*.cpp")
file (GLOB_RECURSE MATH_HEADERS "src/*.h")
include_directories ("src")

set (MATH_NAME "Math")
set (MATH_DESCRIPTION "Discrete math library")
set (MATH_VERSION 0.1.2)

set (MATH_LIBRARY "math")
set (MATH_STATIC ${MATH_LIBRARY}-static)
set (MATH_SHARED ${MATH_LIBRARY}-shared)

configure_file (math.pc.in math.pc @ONLY)
install (FILES ${PROJECT_BINARY_DIR}/math.pc DESTINATION lib/pkgconfig)

if (DOXYGEN_FOUND)
    configure_file (Doxyfile.in Doxyfile @ONLY)
    install (DIRECTORY ${PROJECT_BINARY_DIR}/html DESTINATION share/doc/${MATH_LIBRARY}-${MATH_VERSION})
    add_custom_target ("doxygen" ALL "doxygen")
endif ()

add_library (${MATH_LIBRARY} OBJECT ${MATH_SOURCES})
add_library (${MATH_STATIC} STATIC $<TARGET_OBJECTS:${MATH_LIBRARY}>)
add_library (${MATH_SHARED} SHARED $<TARGET_OBJECTS:${MATH_LIBRARY}>)

# Required to reuse OBJECT for SHARED target, cmake version 2.8.9
set_property (TARGET ${MATH_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties (${MATH_STATIC} ${MATH_SHARED} PROPERTIES OUTPUT_NAME ${MATH_LIBRARY})
set_target_properties (${MATH_SHARED} PROPERTIES VERSION ${MATH_VERSION} SOVERSION ${MATH_VERSION})

install (TARGETS ${MATH_STATIC} ${MATH_SHARED} DESTINATION lib)
install (FILES ${MATH_HEADERS} DESTINATION include/${MATH_LIBRARY})
