# cppcolormap cmake module
#
# This module sets the target:
#
#   cppcolormap
#
# In addition, it sets the following variables:
#
#   cppcolormap_FOUND - true if cppcolormap found
#   cppcolormap_VERSION - cppcolormap's version
#   cppcolormap_INCLUDE_DIRS - the directory containing cppcolormap headers
# The following support targets are defined to simplify things:
#
#   cppcolormap::compiler_warnings - enable compiler warnings

include(CMakeFindDependencyMacro)

find_dependency(xtensor)

if(NOT TARGET cppcolormap)
    include("${CMAKE_CURRENT_LIST_DIR}/cppcolormapTargets.cmake")
    get_target_property(cppcolormap_INCLUDE_DIRS cppcolormap INTERFACE_INCLUDE_DIRECTORIES)
endif()

# Define support target "cppcolormap::compiler_warnings"

if(NOT TARGET cppcolormap::compiler_warnings)
    add_library(cppcolormap::compiler_warnings INTERFACE IMPORTED)
    if(MSVC)
        set_property(
            TARGET cppcolormap::compiler_warnings
            PROPERTY INTERFACE_COMPILE_OPTIONS
            /W4)
    else()
        set_property(
            TARGET cppcolormap::compiler_warnings
            PROPERTY INTERFACE_COMPILE_OPTIONS
            -Wall -Wextra -pedantic -Wno-unknown-pragmas)
    endif()
endif()
