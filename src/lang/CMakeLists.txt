cmake_minimum_required(VERSION 2.8)

project(intent-lang)

include(../../cmake/common.cmake)

include_directories(..)

include(../../cmake/boost.cmake)

file(GLOB SRC_LIST "*.cpp")

set(TARGET_NAME ${PROJECT_NAME})
add_library(${PROJECT_NAME} ${SRC_LIST})
