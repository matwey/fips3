cmake_minimum_required(VERSION 2.8.11)
project(fips)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Test)

include_directories ("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(fips ${SOURCES})
target_link_libraries(fips Qt5::Widgets)

enable_testing()
add_executable(test_fits test/fits.cpp src/fits.cpp src/exception.cpp src/abstractfitsstorage.cpp src/mmapfitsstorage.cpp)
set_target_properties(test_fits PROPERTIES COMPILE_FLAGS -DPROJECT_ROOT='"${PROJECT_SOURCE_DIR}"')
target_link_libraries(test_fits Qt5::Widgets Qt5::Test)
add_test(test_fits test_fits)
