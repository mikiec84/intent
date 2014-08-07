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

set(CPACK_PACKAGE_NAME "intent")
set(CPACK_PACKAGE_VENDOR "intentlang.org")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Intent Programming Ecosystem")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "intent")
set(CPACK_GENERATOR "TGZ;ZIP;STGZ")
set(CPACK_PACKAGE_CONTACT "daniel.hardman@gmail.com")
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    execute_process(COMMAND which rpmbuild OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE found)
    if (NOT "${found}" STREQUAL "")
        set(CPACK_GENERATOR "${CPACK_GENERATOR};RPM")
    endif()
    set(CPACK_GENERATOR "${CPACK_GENERATOR};DEB")
endif()

 # This must always be last!
 include(CPack)
