cmake_minimum_required(VERSION 2.8)



message("Using compiler: ${BoldCyan}${CMAKE_CXX_COMPILER}${ColourReset}")
if(("${CMAKE_BUILD_TYPE}" MATCHES "test_.*") OR ("${CMAKE_BUILD_TYPE}" STREQUAL "analyze"))
        if(${CLANG})
        else()
                message(FATAL_ERROR "${BoldRed}You must use clang for ${CMAKE_BUILD_TYPE} builds.${ColourReset}")
        endif()
endif()

#
# General flags
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wreturn-type -Wno-write-strings -Wno-unused-result -pipe")

project(intent)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
