cmake_minimum_required(VERSION 2.8)

project(testutil)
set(TARGET_NAME ${PROJECT_NAME})

include_directories(..)

file(GLOB SRC_LIST "*.cpp")

include(../../cmake/boost.cmake)

add_library(${TARGET_NAME} ${SRC_LIST})
