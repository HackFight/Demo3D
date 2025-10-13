cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)

project(Demo3D)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Dependencies.cmake)
add_subdirectory(thirdParty/glad)

add_subdirectory(Core)
add_subdirectory(App)