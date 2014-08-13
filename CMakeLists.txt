cmake_minimum_required(VERSION 2.8)

project(intent)

if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    message(FATAL_ERROR "You must compile with Clang, due to LLVM dependencies.")
endif ()

# Include our own custom "find" modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Prefer static linking. We do this because it simplifies distribution, and it
# makes our binaries less dependent and less likely to be influenced by what's
# installed on target systems. The extra cost in file size does not seem like
# a significant drawback.
if (NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Windows"))
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ${CMAKE_FIND_LIBRARY_SUFFIXES})
    list(REMOVE_DUPLICATES CMAKE_FIND_LIBRARY_SUFFIXES)
endif()

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
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(CPACK_INSTALLER_PROGRAM "${CMAKE_CURRENT_LIST_DIR}/tools/PackageMaker.app/Contents/MacOS/PackageMaker" CACHE STRING "" FORCE)
    message("path to binary = ${CPACK_INSTALLER_PROGRAM}")
    set(CPACK_GENERATOR "${CPACK_GENERATOR};PackageMaker")
endif()

 # This must always be last!
 include(CPack)
