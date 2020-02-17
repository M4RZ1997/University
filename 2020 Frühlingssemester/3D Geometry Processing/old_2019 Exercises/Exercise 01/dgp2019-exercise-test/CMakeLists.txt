cmake_minimum_required(VERSION 3.0)

get_filename_component(PROJECTNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECTNAME})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# set(CMAKE_FIND_QUIETLY TRUE)
include(cmake/ConfigureCompiler.cmake)
include(cmake/ConfigureEigen.cmake)

#================================================================
# Labs and exercises
#================================================================
add_subdirectory(lab0_eigen)

