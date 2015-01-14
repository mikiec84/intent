cmake_minimum_required(VERSION 2.8)

project(perftest)

include(../../cmake/common.cmake)

find_package(PCRE REQUIRED)
include_directories(
    ..
    ${PCRE_INCLUDE_DIRS}
)

file(GLOB SRC_LIST "*.cpp")
file(GLOB HEADERS "*.h")
list(APPEND SRC_LIST ${HEADERS})

add_executable(${PROJECT_NAME} ${SRC_LIST})

include(../../cmake/boost.cmake)

target_link_libraries(${PROJECT_NAME}
    intent-core
    ${Boost_LIBRARIES}
    ${PCRE_LIBRARIES}
    ${CARES_LIBRARIES}
)

install(TARGETS
    perftest
    RUNTIME DESTINATION bin)
