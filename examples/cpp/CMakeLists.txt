cmake_minimum_required(VERSION 3.19..3.21)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(cppcolormap)
    find_package(cppcolormap REQUIRED CONFIG)
endif()

set(MYPROJECT "${PROJECT_NAME}-examples")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Catch2 REQUIRED)
find_package(xtensor REQUIRED)

add_library(mytarget INTERFACE IMPORTED)

target_link_libraries(mytarget INTERFACE
    ${PROJECT_NAME}
    ${PROJECT_NAME}::compiler_warnings
    Catch2::Catch2)

file(GLOB APP_SOURCES *.cpp)

foreach(mysource ${APP_SOURCES})
    string(REPLACE ".cpp" "" myexec ${mysource})
    get_filename_component(myexec ${myexec} NAME)
    add_executable(${myexec} ${mysource})
    target_link_libraries(${myexec} PRIVATE mytarget)
    add_test(NAME ${myexec} COMMAND ${myexec})
endforeach()
