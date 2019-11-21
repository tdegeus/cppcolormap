cmake_minimum_required(VERSION 3.1)

project(example)

find_package(xtl REQUIRED)
find_package(xtensor REQUIRED)
find_package(cppcolormap REQUIRED)

add_executable(example match.cpp)

if(MSVC)
    target_compile_options(example PRIVATE /EHsc /MP /bigobj)
    set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
    (CMAKE_CXX_COMPILER_ID MATCHES "Intel" AND NOT WIN32))
    target_compile_options(example PRIVATE -march=native -std=c++14)
endif()

target_link_libraries(example
    PRIVATE
    xtensor
    cppcolormap)
