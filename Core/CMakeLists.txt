cmake_minimum_required(VERSION 3.10)

project(Core)

# We create MY_SOURCES, a list of all the source files of the project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(Core STATIC)
target_sources(Core PRIVATE ${MY_SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(Core glfw)
target_link_libraries(Core glad)
target_link_libraries(Core glm)

# We include all the files in the include directory
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)