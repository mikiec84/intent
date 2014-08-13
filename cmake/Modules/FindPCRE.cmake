# Find the native PCRE includes and library
#
#  PCRE_INCLUDE_DIRS - where to find pcre.h, etc.
#  PCRE_LIBRARIES    - List of libraries when using pcre.
#  PCRE_FOUND        - True if pcre found.

if (PCRE_INCLUDE_DIRS)
    set(PCRE_FIND_QUIETLY ON)
endif()

find_path(PCRE_INCLUDE_DIR pcre.h)

find_library(PCRE_LIBRARY NAMES pcre)
find_library(PCRECPP_LIBRARY NAMES pcrecpp)
# The following two libraries are not currently used, and are not installed
# by default on linux (though they appear to exist in homebrew installs).
#find_library(PCRE16_LIBRARY NAMES pcre16)
#find_library(PCRE32_LIBRARY NAMES pcre32)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PCRE DEFAULT_MSG
    PCRE_LIBRARY
    PCRE_INCLUDE_DIR
    PCRECPP_LIBRARY
    #PCRE16_LIBRARY
    #PCRE32_LIBRARY
)

if(PCRE_FOUND)
    set(PCRE_LIBRARIES
        # We place the cpp library first, because any unfulfilled symbols in it
        # are likely to be found in the base library which follows. Order used
        # to be less important, but recent changes in some linkers have made
        # this matter more.
        ${PCRECPP_LIBRARY}
        ${PCRE_LIBRARY}
        # ${PCRE16_LIBRARY}
        # ${PCRE32_LIBRARY}
    )
    set(PCRE_INCLUDE_DIRS ${PCRE_INCLUDE_DIR})
else()
    set(PCRE_LIBRARIES)
    set(PCRE_INCLUDE_DIRS)
endif()

mark_as_advanced( PCRE_LIBRARIES PCRE_INCLUDE_DIRS )
