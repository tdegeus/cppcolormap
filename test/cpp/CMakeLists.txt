cmake_minimum_required(VERSION 3.1)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(cppcolormap-test)
    find_package(cppcolormap REQUIRED CONFIG)
endif()

set(test_name "unit-tests")

find_package(Catch2 REQUIRED)

add_executable(${test_name} main.cpp)

target_link_libraries(${test_name} PRIVATE Catch2::Catch2)
target_link_libraries(${test_name} PRIVATE cppcolormap)

add_test(NAME ${test_name} COMMAND ${test_name})
