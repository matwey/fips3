cmake_minimum_required(VERSION 2.8.11)
project(fips)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(fips ${SOURCES})
target_link_libraries(fips Qt5::Widgets)
