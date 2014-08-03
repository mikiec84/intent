set(BUILD_TYPES
    "Debug|Release|analyze|leaks|profile|test_undefined|test_addr|test_thread|test_mem")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: ${BUILD_TYPES}" FORCE)
endif()

if(NOT ("${CMAKE_BUILD_TYPE}" MATCHES "Debug|Release"))
    if("${CMAKE_BUILD_TYPE}" MATCHES "[A-Z]")
        string(TOLOWER "${CMAKE_BUILD_TYPE}" lower)
        set(CMAKE_BUILD_TYPE lower)
    endif()
endif()

# Confirm a valid build type was supplied
if(NOT "${CMAKE_BUILD_TYPE}" MATCHES "^${BUILD_TYPES}$")
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}.")
endif()

# Report the build type
if(NOT ("$ENV{CMAKE_BUILDTYPE_SHOWN}" STREQUAL "1"))
    set(ENV{CMAKE_BUILDTYPE_SHOWN} 1)
    message("Build type: ${CMAKE_BUILD_TYPE}")
    message("Supported build types: ${BUILD_TYPES}.\n"
            "(Select with -DCMAKE_BUILD_TYPE=x...)\n")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "leaks")
    message("NOTE - Must set env var to enable leak detection, such as:\n"
            "   $ env HEAPCHECK=normal ./${PROJECT_NAME}")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "profile")
    message("NOTE - Must set env var to enable profiling, such as:\n"
            "   $ env CPUPROFILE=prof.out ./${PROJECT_NAME}$\n"
            "   $ env HEAPPROFILE=hprof.out ./${PROJECT_NAME}")
endif()

set(WARNING_FLAGS "-Wall -Wextra -Werror -Wno-unused-result -Wno-unused-value")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wno-unused-parameter -Wno-write-strings")
set(WARNING_FLAGS "${WARNING_FLAGS} -Qunused-arguments -Wno-missing-field-initializers")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -pipe ${WARNING_FLAGS}")

# Regardless of whether we're in release or debug mode, always generate
# a map file and make debugging possible.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer")

# In release mode, still generate a map file and make debugging possible.
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ${CMAKE_CXX_FLAGS_RELEASE}")

