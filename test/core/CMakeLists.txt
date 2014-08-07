cmake_minimum_required(VERSION 2.8)

project(intent-core-testrunner)

include(../../cmake/common.cmake)

include_directories(
    ../../src
    ${gtest_SOURCE_DIR}/include
)

file(GLOB SRC_LIST "*.cpp")

if (BUILD_TESTRUNNERS)
    add_executable(${PROJECT_NAME} ${SRC_LIST})

    include(../../cmake/boost.cmake)

    target_link_libraries(${PROJECT_NAME}
        intent-core
        gtest
        gtest_main
        ${Boost_LIBRARIES}
    )
    add_test(NAME "all_tests_in_${PROJECT_NAME}" COMMAND ${PROJECT_NAME})
else()
    message("${PROJECT_NAME} is not built in ${CMAKE_BUILD_TYPE} builds.")
    add_custom_target(${PROJECT_NAME} ${SRC_LIST})
endif()
