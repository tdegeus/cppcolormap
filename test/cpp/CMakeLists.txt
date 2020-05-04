cmake_minimum_required(VERSION 3.1)

project(test)
find_package(cppcolormap REQUIRED)
add_executable(test main.cpp)
target_link_libraries(test PRIVATE cppcolormap)
