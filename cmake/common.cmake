set(DEBUG_SANITIZERS integer undefined unsigned-integer-overflow address)
set(BUILD_TESTRUNNERS 1)

# Figure out what build types are possible.
if ("$ENV{INTENT_BUILD_TYPES}" STREQUAL "")
    set(BUILD_TYPES "Debug|Release|analyze")

    # Figure out fully qualified path to the detect_sanitizers script.
    # There are better ways to do this in recent versions of cmake, but
    # we have to use this awkward way for 2.8 compatibility.
    get_filename_component(MY_PATH ${CMAKE_CURRENT_LIST_FILE} ABSOLUTE)
    string(FIND ${MY_PATH} "/" LAST_SLASH REVERSE)
    string(SUBSTRING ${MY_PATH} 0 ${LAST_SLASH} MY_PATH)
    execute_process(COMMAND python ${MY_PATH}/detect_sanitizers
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE SANITIZERS)
    separate_arguments(SANITIZERS)
    foreach(sanitizer ${SANITIZERS})
        if (NOT "${DEBUG_SANITIZERS}" MATCHES ${sanitizer})
            set(BUILD_TYPES "${BUILD_TYPES}|sanitize-${sanitizer}")
        endif()
    endforeach()
    set(ENV{INTENT_BUILD_TYPES} "${BUILD_TYPES}")
    set(ENV{INTENT_SANITIZERS} "${SANITIZERS}")
else()
    set(BUILD_TYPES "$ENV{INTENT_BUILD_TYPES}")
    set(SANITIZERS "$ENV{INTENT_SANITIZERS}")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Valid build types are: ${BUILD_TYPES}" FORCE)
endif()

if (NOT ("${CMAKE_BUILD_TYPE}" MATCHES "Debug|Release"))
    if("${CMAKE_BUILD_TYPE}" MATCHES "[A-Z]")
        string(TOLOWER "${CMAKE_BUILD_TYPE}" lower)
        set(CMAKE_BUILD_TYPE lower)
    endif()
endif()

# Confirm that a valid build type was supplied.
if (NOT "${CMAKE_BUILD_TYPE}" MATCHES "^(${BUILD_TYPES})$")
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}.")
endif()

# Report the build type.
if (NOT ("$ENV{CMAKE_BUILDTYPE_SHOWN}" STREQUAL "1"))
    set(ENV{CMAKE_BUILDTYPE_SHOWN} 1)
    message("\nBuild type: ${CMAKE_BUILD_TYPE}")
    message("  Supported: ${BUILD_TYPES}\n"
            "  (Change with -DCMAKE_BUILD_TYPE=x...)\n")
endif()

set(WARNING_FLAGS "-Wall -Wextra -Werror -Wno-unused-result -Wno-unused-value")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wno-unused-parameter -Wno-write-strings")
set(WARNING_FLAGS "${WARNING_FLAGS} -Qunused-arguments -Wno-missing-field-initializers")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -pipe ${WARNING_FLAGS}")

# Regardless of whether we're in release or debug mode, always enable debugging.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")

# In release mode, still generate a map file; go for moderate optimization.
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 ${CMAKE_CXX_FLAGS_RELEASE}")

# In debug mode, make sure that DEBUG is defined.
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG ${CMAKE_CXX_FLAGS_DEBUG}")

if (${CMAKE_BUILD_TYPE} MATCHES "Debug|sanitize-.*")
    set(SFLAGS "")
    foreach(sanitizer ${DEBUG_SANITIZERS})
        # If this sanitizer is supported, turn it on, unless it's
        # "address" -- that one conflicts with "thread" and
        # "memory", so we can only do it in Debug builds.
        if ("${SANITIZERS}" MATCHES ${sanitizer})
            if (NOT ${sanitizer} STREQUAL "address")
              set(SFLAGS "${SFLAGS},${sanitizer}")
            endif()
        endif()
    endforeach()
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        if ("${SANITIZERS}" MATCHES "address")
            set(SFLAGS "${SFLAGS},address")
        endif()
    elseif (${CMAKE_BUILD_TYPE} STREQUAL "sanitize-thread")
        set(SFLAGS "${SFLAGS},thread -fPIC -pie")
        #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC -pie")
        set(BUILD_TESTRUNNERS 0)
    elseif (${CMAKE_BUILD_TYPE} STREQUAL "sanitize-memory")
        set(SFLAGS "${SFLAGS},memory -fPIC -pie")
        #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC -pie")
        set(BUILD_TESTRUNNERS 0)
    endif()
    if (NOT ${SFLAGS} STREQUAL "")
        string(SUBSTRING ${SFLAGS} 1 -1 SFLAGS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SFLAGS}")
    endif()
elseif (${CMAKE_BUILD_TYPE} STREQUAL "analyze")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --analyze")
    set(BUILD_TESTRUNNERS 0)
endif()

