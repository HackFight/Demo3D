cmake_minimum_required(VERSION 3.10)

project(Core)

# We create MY_SOURCES, a list of all the source files of the project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(Core "${MY_SOURCES}")

# We include all the files in the include directory
target_include_directories(Core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")