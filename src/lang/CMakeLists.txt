cmake_minimum_required(VERSION 2.8)

project(intent-lang)

include(../../cmake/common.cmake)

include_directories(..)

include(../../cmake/boost.cmake)

file(GLOB SRC_LIST "*.cpp")
file(GLOB HEADERS "*.h")
list(APPEND SRC_LIST ${HEADERS})

add_library(${PROJECT_NAME} ${SRC_LIST})
