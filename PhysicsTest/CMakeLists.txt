cmake_minimum_required(VERSION 3.10)

#Set this to true for shipping!
set(PRODUCTION_BUILD FALSE CACHE BOOL "Make this a production build" FORCE)


project(PhysicsTest)

# We create MY_SOURCES, a list of all the source files of the project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(PhysicsTest ${MY_SOURCES})

# We make the RESOURCES_PATH constant that sends to the resources folder
if(PRODUCTION_BUILD)
	target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCES_PATH="./resources/") #Relative path
	target_compile_definitions(${PROJECT_NAME} PUBLIC PRODUCTION_BUILD=1)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/") #Absolute path... ONLY DURING DEVELOPEMENT!
	target_compile_definitions(${PROJECT_NAME} PUBLIC PRODUCTION_BUILD=0)
endif()

target_link_libraries(PhysicsTest Core)

# We include all the files in the include directory
target_include_directories(PhysicsTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)