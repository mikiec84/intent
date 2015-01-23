cmake_minimum_required(VERSION 2.8)

project(intent-core)

include(../../cmake/common.cmake)

include_directories(..)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

file(GLOB_RECURSE SRC_LIST "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
list(APPEND SRC_LIST ${HEADERS})

include(../../cmake/boost.cmake)
include(../../cmake/asio.cmake)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

#add_library(${PROJECT_NAME}-shared SHARED ${SRC_LIST})
#target_link_libraries(${PROJECT_NAME}-shared
#    ${Boost_LIBRARIES}
#)

install(TARGETS intent-core ARCHIVE DESTINATION lib)
#install(TARGETS intent-core-shared ARCHIVE DESTINATION lib)

