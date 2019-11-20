
cmake_minimum_required(VERSION 3.0)

# Options
# =======

project(cppcolormap)

# Internals
# =========

set(CPPCOLORMAP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Version
# =======

file(
    STRINGS "${CPPCOLORMAP_INCLUDE_DIR}/cppcolormap.h" cppcolormap_version_defines
    REGEX "#define CPPCOLORMAP_VERSION_(MAJOR|MINOR|PATCH)")

foreach(ver ${cppcolormap_version_defines})
    if(ver MATCHES "#define CPPCOLORMAP_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(CPPCOLORMAP_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()

set(CPPCOLORMAP_VERSION
    ${CPPCOLORMAP_VERSION_MAJOR}.${CPPCOLORMAP_VERSION_MINOR}.${CPPCOLORMAP_VERSION_PATCH})

message(STATUS "Building cppcolormap v${CPPCOLORMAP_VERSION}")

# Build
# =====

set(CPPCOLORMAP_HEADERS
    ${CPPCOLORMAP_INCLUDE_DIR}/cppcolormap.h)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CPPCOLORMAP_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>)

# Installation
# ============

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets)

export(
    EXPORT ${PROJECT_NAME}-targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

install(
    FILES ${CPPCOLORMAP_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPPCOLORMAP_CMAKECONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE
    STRING "install path for ${PROJECT_NAME}Config.cmake")

configure_package_config_file(
    "CPPCOLORMAPConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CPPCOLORMAP_CMAKECONFIG_INSTALL_DIR})

set(_CPPCOLORMAP_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})

unset(CMAKE_SIZEOF_VOID_P)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${CPPCOLORMAP_VERSION}
    COMPATIBILITY AnyNewerVersion)

set(CMAKE_SIZEOF_VOID_P ${_CPPCOLORMAP_CMAKE_SIZEOF_VOID_P})

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CPPCOLORMAP_CMAKECONFIG_INSTALL_DIR})

install(
    EXPORT ${PROJECT_NAME}-targets
    FILE "${PROJECT_NAME}Targets.cmake"
    DESTINATION ${CPPCOLORMAP_CMAKECONFIG_INSTALL_DIR})

configure_file(
    "cppcolormap.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
     @ONLY)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")
