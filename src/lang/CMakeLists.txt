cmake_minimum_required(VERSION 2.8)

project(intent-lang)

include(../../cmake/common.cmake)

include_directories(..)

include(../../cmake/boost.cmake)

file(GLOB_RECURSE SRC_LIST "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
list(APPEND SRC_LIST ${HEADERS})
file(GLOB_RECURSE TUPLES "*.tuples")
list(APPEND SRC_LIST ${TUPLES})

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

#add_library(${PROJECT_NAME}-shared SHARED ${SRC_LIST})
#target_link_libraries(${PROJECT_NAME}-shared
#    intent-core
#    ${Boost_LIBRARIES}
#)

install(TARGETS intent-lang ARCHIVE DESTINATION lib)
#install(TARGETS intent-lang-shared ARCHIVE DESTINATION lib)

install(FILES
    lexer.h
    DESTINATION include)

