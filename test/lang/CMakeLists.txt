cmake_minimum_required(VERSION 2.8)

project(intent-lang-testrunner)

include(../../cmake/common.cmake)

include_directories(
    ../../src
    ${gtest_SOURCE_DIR}/include
)

include(../../cmake/boost.cmake)

file(GLOB SRC_LIST "*.cpp")

if (BUILD_TESTRUNNERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize-blacklist=${intent-core_SOURCE_DIR}/../../cmake/sanitize-blacklist.txt")
    add_executable(${PROJECT_NAME} ${SRC_LIST})

    target_link_libraries(${PROJECT_NAME}
        intent-core
        intent-lang
        test-util
        gtest
        gtest_main
        ${Boost_LIBRARIES}
    )
    add_test(NAME "all_tests_in_${PROJECT_NAME}" COMMAND ${PROJECT_NAME})
else()
    message("${PROJECT_NAME} is not build in ${CMAKE_BUILD_TYPE} builds.")
    add_custom_target(${PROJECT_NAME} ${SRC_LIST})
endif()
# Make cmake think other files are significant, even if they are not required
# for a build. This causes cmake to place the files in a project tree where
# an IDE can interact with them.
file(GLOB_RECURSE data "data/*")
add_custom_target(data SOURCES ${data})

