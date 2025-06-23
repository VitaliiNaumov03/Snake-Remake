cmake_minimum_required(VERSION 3.10)
project(Snake_Remake LANGUAGES CXX)

#C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Compiler flags
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O2")

#Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#.exe directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

#Directories with source code (.cpp)
set(SOURCE_DIRS
    Functions
    Food
    Snake
    ColorController
    ScoreController
    ResourceManager
    RayTools)

set(SOURCES main.cpp)
list(APPEND SOURCES Compiling/resources.rc) #Adding icon and info through rc file

#Adding all cpp files to SOURCES
foreach(DIR ${SOURCE_DIRS})
    file(GLOB_RECURSE DIR_SOURCES "${CMAKE_SOURCE_DIR}/${DIR}/*.cpp")
    list(APPEND SOURCES ${DIR_SOURCES})
endforeach()

#Disabling console in release version
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(EXECUTABLE_NAME "Snake Remake")
    add_executable(Snake_Remake WIN32 ${SOURCES})
else()
    set(EXECUTABLE_NAME "Snake Remake (DEBUG)")
    add_executable(Snake_Remake ${SOURCES})
endif()

set_target_properties(Snake_Remake PROPERTIES OUTPUT_NAME "${EXECUTABLE_NAME}")

#Finding RayLib
set(RAYLIB_PATH "${CMAKE_SOURCE_DIR}/Libs")
find_library(RAYLIB_LIBRARY
    NAMES raylib libraylib
    PATHS ${RAYLIB_PATH}
    NO_DEFAULT_PATH
)

#If RayLib wasn't found
if(NOT RAYLIB_LIBRARY)
    message(FATAL_ERROR "RayLib was not found in \"${RAYLIB_PATH}\"")
endif()

#Link libraries
target_link_libraries(Snake_Remake PRIVATE ${RAYLIB_LIBRARY} winmm)