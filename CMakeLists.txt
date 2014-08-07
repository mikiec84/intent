cmake_minimum_required(VERSION 2.8)

project(intent)

if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    message(FATAL_ERROR "You must compile with Clang, due to LLVM dependencies.")
endif ()

enable_testing()

add_subdirectory(src)
add_subdirectory(test)

# Make cmake think other files are significant, even if they are not required
# for a build. This causes cmake to place the files in a project tree where
# an IDE can interact with them.
file(GLOB_RECURSE assets "assets/*")
file(GLOB_RECURSE snippets "snippets/*")
file(GLOB_RECURSE grammar "grammar/*")
file(GLOB_RECURSE cmake "cmake/*")
add_custom_target(misc SOURCES
    .travis.yml
    cmake/run_travis_build
    README.md
    LICENSE
    .gitignore
    ${assets}
    ${snippets}
    ${grammar}
    ${cmake}
)

set(CPACK_PACKAGE_NAME "Intent")
set(CPACK_PACKAGE_VENDOR "intentlang.org")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Intent Programming Ecosystem")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "intent")
set(CPACK_GENERATOR "TGZ;ZIP;STGZ")
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CPACK_GENERATOR "${CPACK_GENERATOR};DEB;RPM")
endif()

 # This must always be last!
 include(CPack)
