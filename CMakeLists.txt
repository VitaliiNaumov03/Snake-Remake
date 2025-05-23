cmake_minimum_required(VERSION 3.10)
project(Snake_Remake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O2 -mwindows")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
endif()

#Directories with source code (.cpp)
set(SOURCE_DIRS
    Functions
    Food
    Snake
    ColorController
    ResourceManager
    RayTools)

set(EXECUTABLE_NAME "Snake Remake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}) #.exe folder
link_directories(${CMAKE_SOURCE_DIR}/Libs) #Libraries location

set(SOURCES main.cpp)
foreach(DIR ${SOURCE_DIRS})
    file(GLOB_RECURSE DIR_SOURCES "${CMAKE_SOURCE_DIR}/${DIR}/*.cpp")
    list(APPEND SOURCES ${DIR_SOURCES})
endforeach()

add_executable(Snake_Remake ${SOURCES})
set_target_properties(Snake_Remake PROPERTIES OUTPUT_NAME "${EXECUTABLE_NAME}")
target_link_libraries(Snake_Remake raylib winmm)