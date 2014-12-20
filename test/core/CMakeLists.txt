cmake_minimum_required(VERSION 2.8)

project(intent-core-testrunner)

include(../../cmake/common.cmake)

file(GLOB_RECURSE SRC_LIST "*.cpp")

if (BUILD_TESTRUNNERS)

    find_package(PCRE REQUIRED)

    include_directories(
        ../../src
        ${gtest_SOURCE_DIR}/include
        ${PCRE_INCLUDE_DIRS}
    )

    add_executable(${PROJECT_NAME} ${SRC_LIST})

    # Suppress sanitizer warnings for a few places where we are deliberately
    # doing funky stuff in tests.
    if (${CMAKE_CXX_FLAGS} MATCHES "-fsanitize=")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize-blacklist=${CMAKE_CURRENT_BINARY_DIR}/sanitizer-blacklist.txt")
        configure_file(${CMAKE_CURRENT_LIST_DIR}/sanitizer-blacklist.txt.in ${CMAKE_CURRENT_BINARY_DIR}/sanitizer-blacklist.txt @ONLY)
    endif()

    include(../../cmake/boost.cmake)

    target_link_libraries(${PROJECT_NAME}
        intent-core
        test-util
        gtest
        gtest_main
        ${Boost_LIBRARIES}
        ${PCRE_LIBRARIES}
    )
    add_test(NAME "all_tests_in_${PROJECT_NAME}" COMMAND ${PROJECT_NAME})
else()
    message("${PROJECT_NAME} is not built in ${CMAKE_BUILD_TYPE} builds.")
    add_custom_target(${PROJECT_NAME} ${SRC_LIST})
endif()
