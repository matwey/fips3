cmake_minimum_required(VERSION 2.8.11)
project(fips)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)
add_executable(fips ${SOURCES})
target_link_libraries(fips Qt5::Widgets)

enable_testing()
add_executable(test_fits test/fits.cpp src/fits.cpp src/exception.cpp src/abstractfitsstorage.cpp src/mmapfitsstorage.cpp)
set_target_properties(test_fits PROPERTIES COMPILE_FLAGS -DPROJECT_ROOT='"${PROJECT_SOURCE_DIR}"')
# Linked libraries affect ability to run without X11 display.
#
# http://doc.qt.io/qt-5/qtest.html#QTEST_MAIN
#
# If QT_WIDGETS_LIB is defined, the application object will be a QApplication,
# if QT_GUI_LIB is defined, the application object will be a QGuiApplication,
# otherwise it will be a QCoreApplication.
target_link_libraries(test_fits Qt5::Test)
add_test(test_fits test_fits)

add_executable(test_openglshaderuniforms test/openglshaderuniforms.cpp src/openglshaderunifroms.cpp)
set_target_properties(test_openglshaderuniforms PROPERTIES COMPILE_FLAGS -DPROJECT_ROOT='"${PROJECT_SOURCE_DIR}"')
target_link_libraries(test_openglshaderuniforms Qt5::Test)
add_test(test_openglshaderuniforms test_openglshaderuniforms)
