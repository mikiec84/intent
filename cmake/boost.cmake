# See http://j.mp/1qS8GbQ and http://j.mp/1uV5U7A for info about how to use
# boost with cmake.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.55.0 REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})

# To link against boost libraries, simply add ${Boost_LIBRARIES} to link
# targets, like this:

#target_link_libraries(${TARGET_NAME}
#    foo
#    ${Boost_LIBRARIES}
#)
