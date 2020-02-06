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

include(CMakeFindDependencyMacro)

find_dependency(xtensor)

if(NOT TARGET cppcolormap)
    include("${CMAKE_CURRENT_LIST_DIR}/cppcolormapTargets.cmake")
    get_target_property(cppcolormap_INCLUDE_DIRS cppcolormap INTERFACE_INCLUDE_DIRECTORIES)
endif()
