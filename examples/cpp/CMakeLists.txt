cmake_minimum_required(VERSION 3.1)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(cppcolormap-exmaples)
    find_package(cppcolormap REQUIRED CONFIG)
endif()

set(name "match")
add_executable(${name} match.cpp)
target_link_libraries(${name} PRIVATE cppcolormap)
add_test(NAME ${name} COMMAND ${name})
