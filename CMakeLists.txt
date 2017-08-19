cmake_minimum_required(VERSION 2.8.11)
cmake_policy(SET CMP0048 NEW)
project(fips LANGUAGES C CXX VERSION 3.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

if(Qt5Widgets_VERSION VERSION_LESS 5.5)
	message(FATAL_ERROR "Qt5 5.5 or later is required")
endif(Qt5Widgets_VERSION VERSION_LESS 5.5)

include_directories ("${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)

if(APPLE)
	set(TARGET Fips)

	set(RESOURCES_DIR "${PROJECT_SOURCE_DIR}/dist/macos")
	set(ICON_NAME fips)
	set(ICON_FILE "${RESOURCES_DIR}/${ICON_NAME}.icns")
	set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	list(APPEND SOURCES ${ICON_FILE})
else()
	set(TARGET fips)
endif(APPLE)

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} Qt5::Widgets)

if(APPLE)
	set(DEVELOPMENT_PROJECT_NAME Fips)
	set(DEVELOPMENT_TEAM_ID matwey)
	set(PRODUCT_NAME Fips)
	set(EXECUTABLE_NAME Fips)
	set(MACOSX_BUNDLE_EXECUTABLE_NAME Fips)
	set(MACOSX_BUNDLE_INFO_STRING com.matwey.fips)
	set(MACOSX_BUNDLE_GUI_IDENTIFIER ${MACOSX_BUNDLE_INFO_STRING})
	set(MACOSX_BUNDLE_BUNDLE_NAME Fips)
	set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME})
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
	set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
	set(MACOSX_BUNDLE_COPYRIGHT Copyright 2017, Matwey Kornilov, Konstantin Malanchev)
	set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})

	set_target_properties(${TARGET} PROPERTIES MACOSX_BUNDLE TRUE)
	set_target_properties(${TARGET} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${RESOURCES_DIR}/Info.plist.in")
	add_custom_command(TARGET ${TARGET}
		POST_BUILD
		COMMAND macdeployqt ARGS "${CMAKE_CURRENT_BINARY_DIR}/Fips.app" -always-overwrite
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

	install(TARGETS ${TARGET} BUNDLE DESTINATION .)
	install(FILES LICENSE.txt DESTINATION .)

	set(CPACK_PACKAGE_NAME Fips)
	set(CPACK_BUNDLE_NAME Fips)
	set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
	set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
	set(CPACK_PACKAGE_INSTALL_DIRECTORY /Applications)
	set(CPACK_PACKAGE_ICON ${ICON_FILE})
	set(CPACK_PACKAGE_CHECKSUM SHA256)
	set(CPACK_GENERATOR DragNDrop)
	include(CPack)
elseif(UNIX)
	find_path(XDG_PREFIX
		NAMES applications icons
		HINTS ENV XDG_DATA_DIRS
		PATHS /usr/share /usr/local/share
		NO_DEFAULT_PATH)
	set(XDG_DESKTOP_DIR "${XDG_PREFIX}/applications")
	set(XDG_ICONS_DIR "${XDG_PREFIX}/icons")

	configure_file("dist/freedesktop/fips.desktop.in" "fips.desktop" @ONLY)

	install(TARGETS "${TARGET}" DESTINATION bin)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fips.desktop" DESTINATION ${XDG_DESKTOP_DIR})
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/scalable/fips.svg" DESTINATION "${XDG_ICONS_DIR}/hicolor/scalable/apps")
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/128x128/fips.png" DESTINATION "${XDG_ICONS_DIR}/hicolor/128x128/apps")
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/256x256/fips.png" DESTINATION "${XDG_ICONS_DIR}/hicolor/256x256/apps")
endif(APPLE)

enable_testing()
add_executable(test_fits test/fits.cpp src/fits.cpp src/exception.cpp src/abstractfitsstorage.cpp src/mmapfitsstorage.cpp)
target_compile_definitions(test_fits PUBLIC PROJECT_ROOT="${PROJECT_SOURCE_DIR}")
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
target_link_libraries(test_openglshaderuniforms Qt5::Test)
add_test(test_openglshaderuniforms test_openglshaderuniforms)
