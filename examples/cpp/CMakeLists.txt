cmake_minimum_required(VERSION 3.1)

project(example)
find_package(cppcolormap REQUIRED)
add_executable(example match.cpp)
target_link_libraries(example PRIVATE cppcolormap)
