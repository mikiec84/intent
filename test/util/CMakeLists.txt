cmake_minimum_required(VERSION 2.8)

project(test-util)

include(../../cmake/common.cmake)

include_directories(..)

file(GLOB SRC_LIST "*.cpp")
file(GLOB HEADERS "*.h")
list(APPEND SRC_LIST ${HEADERS})

include(../../cmake/boost.cmake)

add_library(${PROJECT_NAME} ${SRC_LIST})
