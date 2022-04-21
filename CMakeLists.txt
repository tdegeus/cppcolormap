cmake_minimum_required(VERSION 3.18..3.21)

project(cppcolormap)

string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)

# Command-line options
# ====================

option(BUILD_ALL "${PROJECT_NAME}: Build tests, Python API & docs" OFF)
option(BUILD_TESTS "${PROJECT_NAME}: Build tests" OFF)
option(BUILD_EXAMPLES "${PROJECT_NAME}: Build examples" OFF)
option(BUILD_PYTHON "${PROJECT_NAME}: Build Python API" OFF)
option(BUILD_DOCS "${PROJECT_NAME}: Build docs" OFF)

if(SKBUILD)
    set(BUILD_ALL 0)
    set(BUILD_TESTS 0)
    set(BUILD_PYTHON 1)
    set(BUILD_DOCS 0)
endif()

# Read version
# ============

if (DEFINED ENV{SETUPTOOLS_SCM_PRETEND_VERSION})
    set(PROJECT_VERSION $ENV{SETUPTOOLS_SCM_PRETEND_VERSION})
    message(STATUS "Building ${PROJECT_NAME} ${PROJECT_VERSION} (read from SETUPTOOLS_SCM_PRETEND_VERSION)")
else()
    execute_process(
        COMMAND python -c "from setuptools_scm import get_version; print(get_version())"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PROJECT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    message(STATUS "Building ${PROJECT_NAME} ${PROJECT_VERSION}")
endif()

# Set target
# ==========

find_package(xtensor REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(${PROJECT_NAME} INTERFACE xtensor)

target_compile_definitions(${PROJECT_NAME} INTERFACE
    ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}")

# Libraries
# =========

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CTest)
include("${PROJECT_NAME}Config.cmake")

# Installation headers / CMake / pkg-config
# =========================================

if(NOT SKBUILD)

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include)

    configure_file("include/cppcolormap.h"
                   "${CMAKE_CURRENT_BINARY_DIR}/cppcolormap.h"
                    @ONLY)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cppcolormap.h"
            DESTINATION "include/")

    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)

    install(
        EXPORT ${PROJECT_NAME}-targets
        FILE "${PROJECT_NAME}Targets.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    set(${PROJECT_NAME}_TMP ${CMAKE_SIZEOF_VOID_P})
    unset(CMAKE_SIZEOF_VOID_P)

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY AnyNewerVersion)

    set(CMAKE_SIZEOF_VOID_P ${${PROJECT_NAME}_TMP})

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" @ONLY)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

endif()

# Build tests
# ===========

if(BUILD_TESTS OR BUILD_ALL)

    enable_testing()
    add_subdirectory(tests/cpp)

endif()

# Build examples
# ==============

if(BUILD_EXAMPLES OR BUILD_ALL)

    enable_testing()
    add_subdirectory(examples/cpp)

endif()

# Build Python API
# ================

if(BUILD_PYTHON OR BUILD_ALL)

    # The C++ functions are build to a library with name "_${PROJECT_NAME}"
    # The Python library simply loads all functions
    set(PYPROJECT_NAME "_${PROJECT_NAME}")

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()

    find_package(pybind11 REQUIRED CONFIG)
    find_package(xtensor-python REQUIRED)

    if (SKBUILD)
        find_package(NumPy REQUIRED)
    else()
        find_package(Python REQUIRED COMPONENTS Interpreter Development NumPy)
    endif()

    pybind11_add_module(${PYPROJECT_NAME} python/main.cpp)

    target_compile_definitions(${PYPROJECT_NAME} PUBLIC VERSION_INFO=${PROJECT_VERSION})
    target_compile_definitions(${PYPROJECT_NAME} PUBLIC CPPCOLORMAP_ENABLE_ASSERT)
    target_link_libraries(${PYPROJECT_NAME} PUBLIC ${PROJECT_NAME} xtensor-python)

    if (SKBUILD)
        target_include_directories(${PYPROJECT_NAME} PUBLIC ${NumPy_INCLUDE_DIRS})
    else()
        target_link_libraries(${PYPROJECT_NAME} PUBLIC ${PROJECT_NAME} pybind11::module Python::NumPy)
    endif()

    if (SKBUILD)
        if(APPLE)
            set_target_properties(${PYPROJECT_NAME} PROPERTIES INSTALL_RPATH "@loader_path/${CMAKE_INSTALL_LIBDIR}")
        else()
            set_target_properties(${PYPROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/${CMAKE_INSTALL_LIBDIR}")
        endif()
        install(TARGETS ${PYPROJECT_NAME} DESTINATION .)
    endif()

endif()

# Build documentation
# ===================

if(BUILD_DOCS OR BUILD_ALL)

    find_package(Doxygen REQUIRED)

    set(DOXYGEN_EXCLUDE_SYMBOLS detail)
    set(DOXYGEN_CASE_SENSE_NAMES YES)
    set(DOXYGEN_USE_MATHJAX YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    set(DOXYGEN_MACRO_EXPANSION YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_GENERATE_XML YES)
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_AS_ERROR YES)
    set(DOXYGEN_ALIASES "license=@par License:")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md")

    set(DOXYGEN_HTML_EXTRA_STYLESHEET
        "${CMAKE_CURRENT_SOURCE_DIR}/docs/doxystyle/doxygen-awesome.css"
        "${CMAKE_CURRENT_SOURCE_DIR}/docs/doxystyle/custom.css")

    set(DOXYGEN_STRIP_FROM_INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
    set(DOXYGEN_STRIP_FROM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
    doxygen_add_docs(html "${CMAKE_CURRENT_SOURCE_DIR}/include" "README.md")

endif()
