
cmake_minimum_required(VERSION 3.0)

# Basic settings
# ==============

project(cppcolormap)

option(BUILD_TESTS "${PROJECT_NAME} Build tests" 0)
option(BUILD_EXAMPLES "${PROJECT_NAME} Build examples" 0)

# Version
# =======

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/cppcolormap.h" cppcolormap_version_defines
     REGEX "#define CPPCOLORMAP_VERSION_(MAJOR|MINOR|PATCH)")

foreach(ver ${cppcolormap_version_defines})
    if(ver MATCHES "#define CPPCOLORMAP_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(CPPCOLORMAP_VERSION_${CMAKE_MATCH_1}
            "${CMAKE_MATCH_2}"
            CACHE INTERNAL "")
    endif()
endforeach()

set(CPPCOLORMAP_VERSION
    ${CPPCOLORMAP_VERSION_MAJOR}.${CPPCOLORMAP_VERSION_MINOR}.${CPPCOLORMAP_VERSION_PATCH})

message(STATUS "Building cppcolormap v${CPPCOLORMAP_VERSION}")

# Set target
# ==========

find_package(xtensor REQUIRED)

add_library(cppcolormap INTERFACE)

target_include_directories(cppcolormap INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(cppcolormap INTERFACE xtensor)

# Installation
# ============

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/cppcolormap.h" DESTINATION include)

install(TARGETS cppcolormap EXPORT cppcolormap-targets)

install(
    EXPORT cppcolormap-targets
    FILE cppcolormapTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cppcolormap")

set(_CPPCOLORMAP ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cppcolormapConfigVersion.cmake"
    VERSION ${CPPCOLORMAP_VERSION}
    COMPATIBILITY AnyNewerVersion)

set(CMAKE_SIZEOF_VOID_P ${_CPPCOLORMAP})

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cppcolormapConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cppcolormapConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cppcolormap")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cppcolormap.pc.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cppcolormap.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cppcolormap.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

# Tests & examples
# ================

set(CTEST_TEST_TARGET_ALIAS RUN_TESTS_AND_EXAMPLES)
include(CTest)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test/cpp)
endif()

if(BUILD_EXAMPLES)
    enable_testing()
    add_subdirectory(examples/cpp)
endif()
