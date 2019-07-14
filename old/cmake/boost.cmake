# See http://j.mp/1qS8GbQ and http://j.mp/1uV5U7A for info about how to use
# boost with cmake.

if (Boost_INCLUDE_DIRS)
    set(Boost_FIND_QUIETLY ON)
endif()

if (${CMAKE_BUILD_TYPE} MATCHES "sanitize-(thread|memory)")
    set(Boost_USE_STATIC_LIBS OFF)
    message("Using dynamic libs for boost for compatibility with sanitizers.")
else()
    set(Boost_USE_STATIC_LIBS ON)
endif()
# This setting should permeate the build everywhere; put it in the cache and don't
# allow it to be overridden.
set(Boost_USE_STATIC_LIBS ${Boost_USE_STATIC_LIBS} CACHE BOOL "link against static libs for boost" FORCE)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ${Boost_USE_STATIC_LIBS})
find_package(Boost 1.48.0 REQUIRED COMPONENTS filesystem system)
set(Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} CACHE STRING "where boost headers are")
include_directories(${Boost_INCLUDE_DIRS})

# We are using a boost extension that's not part of boost proper. It is header-
# only, and stored at src/external/process. Add that path to the includes.
include_directories(${CMAKE_CURRENT_LIST_DIR}/../src/external/boost.process)

# To link against boost libraries, simply add ${Boost_LIBRARIES} to link
# targets, like this:

#target_link_libraries(${TARGET_NAME}
#    foo
#    ${Boost_LIBRARIES}
#)
