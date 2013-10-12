cmake_minimum_required (VERSION 2.8.9)
project (GRAPHENE)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process (COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                     OUTPUT_VARIABLE GCC_VERSION)
    string (STRIP ${GCC_VERSION} GCC_VERSION)

    if (GCC_VERSION VERSION_GREATER 4.7.0)
        set (CMAKE_CXX_FLAGS "-Wall -pedantic -std=gnu++11 -g")
    else ()
        message (FATAL_ERROR "GCC 4.7.0 or higher is required!")
    endif ()
endif ()

find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)

include (FindPkgConfig)
pkg_search_module (SDL2 REQUIRED sdl2)
pkg_search_module (SDL2_IMAGE REQUIRED SDL2_image)

set (MATH_LIBRARY "math")
set (MATH_INCLUDE ${MATH_LIBRARY}/src)
add_subdirectory (${MATH_LIBRARY})

file (GLOB_RECURSE GRAPHENE_SOURCES "src/*.cpp")
file (GLOB_RECURSE GRAPHENE_HEADERS "src/*.h")
include_directories ("src")

set (GRAPHENE_NAME "Graphene")
set (GRAPHENE_DESCRIPTION "OpenGL-based Graphics Engine")
set (GRAPHENE_VERSION 0.0.1)

set (GRAPHENE_LIBRARY "graphene")
set (GRAPHENE_STATIC ${GRAPHENE_LIBRARY}-static)
set (GRAPHENE_SHARED ${GRAPHENE_LIBRARY}-shared)

configure_file (graphene.pc.in graphene.pc @ONLY)
install (FILES ${PROJECT_BINARY_DIR}/graphene.pc DESTINATION lib/pkgconfig)

add_library (${GRAPHENE_LIBRARY} OBJECT ${GRAPHENE_SOURCES})
add_library (${GRAPHENE_STATIC} STATIC $<TARGET_OBJECTS:${GRAPHENE_LIBRARY}>)
add_library (${GRAPHENE_SHARED} SHARED $<TARGET_OBJECTS:${GRAPHENE_LIBRARY}>)

target_link_libraries (${GRAPHENE_STATIC} ${MATH_LIBRARY}-static)
target_link_libraries (${GRAPHENE_STATIC} ${OPENGL_LIBRARIES})
target_link_libraries (${GRAPHENE_STATIC} ${GLEW_LIBRARIES})
target_link_libraries (${GRAPHENE_STATIC} ${SDL2_LIBRARIES})
target_link_libraries (${GRAPHENE_STATIC} ${SDL2_IMAGE_LIBRARIES})

target_link_libraries (${GRAPHENE_SHARED} ${MATH_LIBRARY}-shared)
target_link_libraries (${GRAPHENE_SHARED} ${OPENGL_LIBRARIES})
target_link_libraries (${GRAPHENE_SHARED} ${GLEW_LIBRARIES})
target_link_libraries (${GRAPHENE_SHARED} ${SDL2_LIBRARIES})
target_link_libraries (${GRAPHENE_SHARED} ${SDL2_IMAGE_LIBRARIES})

add_dependencies (${GRAPHENE_LIBRARY} ${MATH_LIBRARY})

# Required to reuse OBJECT for SHARED target, cmake version 2.8.9
set_property (TARGET ${GRAPHENE_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties (${GRAPHENE_STATIC} ${GRAPHENE_SHARED} PROPERTIES OUTPUT_NAME ${GRAPHENE_LIBRARY})
set_target_properties (${GRAPHENE_SHARED} PROPERTIES VERSION ${GRAPHENE_VERSION} SOVERSION ${GRAPHENE_VERSION})

install (TARGETS ${GRAPHENE_STATIC} ${GRAPHENE_SHARED} DESTINATION lib)
install (FILES ${GRAPHENE_HEADERS} DESTINATION include/${GRAPHENE_LIBRARY})
