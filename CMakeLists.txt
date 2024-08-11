cmake_minimum_required(VERSION 3.29.0)

#standard option
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()  
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(DEBUG)
    add_compile_options(-fconcepts-diagnostics-depth=10)
endif()
project(main)

#include dir
set(CMAKE_PREFIX_PATH /home/jujinqian/usr)
include_directories($ENV{INCLUDE} include)
# test
enable_testing()
add_subdirectory(test)

#main
# add_executable(${PROJECT_NAME} main.cpp)
# add_custom_target(run COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
