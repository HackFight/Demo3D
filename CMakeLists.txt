cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 23)

project(Demo3D)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Dependencies.cmake)
add_subdirectory(thirdParty/glad)
add_subdirectory(thirdParty/stb_image)

set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_MINIZIP ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(thirdParty/assimp)

add_subdirectory(Core)
add_subdirectory(App)
add_subdirectory(PhysicsTest)
add_subdirectory(Platformer)