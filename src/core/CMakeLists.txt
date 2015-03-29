cmake_minimum_required(VERSION 2.8)

project(intent-core)

include(../../cmake/common.cmake)

include_directories(
    ..
)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

find_package(PCRE REQUIRED)
include_directories(${PCRE_INCLUDE_DIRS})

file(GLOB_RECURSE SRC_LIST "*.cpp")

file(GLOB_RECURSE HEADERS "*.h")
list(APPEND SRC_LIST ${HEADERS})

file(GLOB_RECURSE TUPLES "*.tuples")
list(APPEND SRC_LIST ${TUPLES})

include(../../cmake/boost.cmake)
include(../../cmake/asio.cmake)

if (${CMAKE_CXX_FLAGS} MATCHES "-fsanitize=")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize-blacklist=${CMAKE_CURRENT_LIST_DIR}/address-sanitizer-blacklist.txt")
endif()

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
set(intall_targets intent-core)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    add_library(${PROJECT_NAME}-shared SHARED ${SRC_LIST})
    target_link_libraries(${PROJECT_NAME}-shared
        ${Boost_LIBRARIES}
        ${PCRE_LIBRARIES}
        ${CURL_LIBRARIES}
    )
    list(APPEND intall_targets intent-core-shared)

endif()

install(TARGETS ${install_targets}
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

