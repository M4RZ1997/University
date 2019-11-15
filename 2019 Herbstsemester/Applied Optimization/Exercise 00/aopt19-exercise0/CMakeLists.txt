cmake_minimum_required(VERSION 3.9)

PROJECT(AOPT_EXERCISES)

if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Build")
else ()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Build/bin")
endif ()


set(AOPT_EXTERNAL "${CMAKE_CURRENT_SOURCE_DIR}/external")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# add library
add_library (AOPT INTERFACE)

add_library (AOPT::AOPT ALIAS AOPT)

#Set target properties
set(EIGEN3_INCLUDE_DIR $<BUILD_INTERFACE:${AOPT_EXTERNAL}/eigen>)

target_include_directories(AOPT
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        ${EIGEN3_INCLUDE_DIR}
)

add_subdirectory(grid_search)
add_subdirectory(csv_exporter)
