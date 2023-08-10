cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(sogen VERSION 0.1 LANGUAGES C CXX)

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

add_subdirectory(src/grami)

add_subdirectory(src/sogen)