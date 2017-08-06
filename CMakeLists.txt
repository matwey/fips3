cmake_minimum_required(VERSION 2.8.11)
cmake_policy(SET CMP0048 NEW)
project(fips LANGUAGES C CXX VERSION 3.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

find_path(XDG_PREFIX
	NAMES applications icons
	HINTS ENV XDG_DATA_DIRS
	PATHS /usr/share /usr/local/share
	NO_DEFAULT_PATH)
set(XDG_DESKTOP_DIR "${XDG_PREFIX}/applications")
set(XDG_ICONS_DIR "${XDG_PREFIX}/icons")

configure_file("dist/freedesktop/fips.desktop.in" "fips.desktop" @ONLY)

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)
add_executable(fips ${SOURCES})
target_link_libraries(fips Qt5::Widgets)
install(TARGETS fips DESTINATION bin)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fips.desktop" DESTINATION ${XDG_DESKTOP_DIR})

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
