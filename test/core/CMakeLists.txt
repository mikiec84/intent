cmake_minimum_required(VERSION 2.8)

project(intent-core-testrunner)

include(../../cmake/common.cmake)

file(GLOB_RECURSE SRC_LIST "*.cpp")

if (BUILD_TESTRUNNERS)

    find_package(PCRE REQUIRED)
    find_package(CURL REQUIRED)

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
        # The items after curl are required by it. See http://packages.ubuntu.com/source/precise/curl
        # Package names on ubuntu: libidn11-dev, zlib1g-dev, libcurl4-openssl-dev,
        # libgcrypt11-dev, libkrb5-dev, libldap2-dev
        curl.so # <-- this forces us to link dynamically, making dependencies go away at compile time
        # I would prefer to link statically, using CURL_STATICLIB. That makes the build env
        # trickier but the deployment/installation easier. Here's what I'd need for a full
        # set of features in curl (the .a forces static link on linux but is wrong on windows, and
        # may be wrong in a given OS instance, depending on what's installed...)
        #idn.a z.a ssl.a crypto.a crypt.a ssh.a krb5 gssapi_krb5 rtmp.a ldap
    )
    add_test(NAME "all_tests_in_${PROJECT_NAME}" COMMAND ${PROJECT_NAME})
else()
    message("${PROJECT_NAME} is not built in ${CMAKE_BUILD_TYPE} builds.")
    add_custom_target(${PROJECT_NAME} ${SRC_LIST})
endif()
