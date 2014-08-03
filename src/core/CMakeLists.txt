cmake_minimum_required(VERSION 2.8)

project(intent-core)

include(../../cmake/common.cmake)

include_directories(..)

file(GLOB SRC_LIST "*.cpp")

include(../../cmake/boost.cmake)

set(TARGET_NAME ${PROJECT_NAME})
add_library(${PROJECT_NAME} ${SRC_LIST})
