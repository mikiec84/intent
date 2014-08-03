cmake_minimum_required(VERSION 2.8)

project(intent-lang-testrunner)

include(../../common.cmake)

include_directories(
    ../../src
    ${gtest_SOURCE_DIR}/include
)

include(../../boost.cmake)

file(GLOB SRC_LIST "*.cpp")
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
    intent-core
    intent-lang
    testutil
    gtest
    gtest_main
    ${Boost_LIBRARIES}
)

add_test(NAME "all_tests_in_${PROJECT_NAME}" COMMAND ${PROJECT_NAME})
