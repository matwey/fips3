cmake_minimum_required(VERSION 3.0.0)
cmake_policy(SET CMP0048 NEW)
project(fips LANGUAGES C CXX VERSION 3.3.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

if(Qt5Widgets_VERSION VERSION_LESS 5.5)
	message(FATAL_ERROR "Qt5 5.5 or later is required")
else(Qt5Widgets_VERSION VERSION_LESS 5.5)
	message(STATUS "Found Qt ${Qt5Widgets_VERSION}")
endif(Qt5Widgets_VERSION VERSION_LESS 5.5)

include_directories ("${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)

set(ICON_NAME fips)
set(DOC_ICON_NAME fips_fits)

if(APPLE)
	set(TARGET Fips)

	set(RESOURCES_DIR ${PROJECT_SOURCE_DIR}/dist/macos)
	set(ICON_FILE ${RESOURCES_DIR}/${ICON_NAME}.icns)
	set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	set(DOC_ICON_FILE ${RESOURCES_DIR}/${DOC_ICON_NAME}.icns)
	set_source_files_properties(${DOC_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	list(APPEND SOURCES ${ICON_FILE} ${DOC_ICON_FILE})
	add_executable(${TARGET} MACOSX_BUNDLE ${SOURCES})
elseif(WIN32)
	set(TARGET fips)

	set(RESOURCES_DIR "${PROJECT_SOURCE_DIR}/dist/windows")
	set(ICON_FILE "${RESOURCES_DIR}/${ICON_NAME}.ico")

	if (NOT PROJECT_VERSION_TWEAK)
		set(PROJECT_VERSION_TWEAK 0)
	endif()
	configure_file(${RESOURCES_DIR}/metafile.rc.in metafile.rc)

	list(APPEND SOURCES metafile.rc)
	add_executable(${TARGET} WIN32 ${SOURCES})
else()
	set(TARGET fips)
	add_executable(${TARGET} ${SOURCES})
endif(APPLE)

target_link_libraries(${TARGET} Qt5::Widgets)
target_compile_definitions(${TARGET} PUBLIC PROJECT_VERSION="${PROJECT_VERSION}")

if(APPLE OR WIN32)
	set(CPACK_PACKAGE_NAME Fips)
	set(CPACK_BUNDLE_NAME Fips)
	set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
	set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
endif()

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
	set(MACOSX_BUNDLE_DOC_ICON_FILE ${DOC_ICON_NAME}.icns)
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
	set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
	set(MACOSX_BUNDLE_COPYRIGHT Copyright 2017, Matwey Kornilov, Konstantin Malanchev)
	set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})

	set_target_properties(${TARGET} PROPERTIES MACOSX_BUNDLE TRUE)
	set_target_properties(${TARGET} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${RESOURCES_DIR}/Info.plist.in")
	if (${CMAKE_VERSION} VERSION_LESS 3.9.0)
		# This is true for Makefile generators only
		set(BUNDLE_DIR ${CMAKE_CURRENT_BINARY_DIR}/Fips.app)
	else()
		set(BUNDLE_DIR $<TARGET_BUNDLE_DIR:${TARGET}>)
	endif(${CMAKE_VERSION} VERSION_LESS 3.9.0)
	add_custom_command(TARGET ${TARGET}
		POST_BUILD
		COMMAND macdeployqt ARGS ${BUNDLE_DIR} -always-overwrite
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

	install(TARGETS ${TARGET} BUNDLE DESTINATION .)
	install(FILES LICENSE.txt DESTINATION .)

	set(CPACK_PACKAGE_INSTALL_DIRECTORY /Applications)
	set(CPACK_PACKAGE_ICON ${ICON_FILE})
	set(CPACK_PACKAGE_CHECKSUM SHA256)
	set(CPACK_GENERATOR DragNDrop)
	include(CPack)
elseif(WIN32)
	include(InstallRequiredSystemLibraries)
	add_custom_command(TARGET ${TARGET}
		POST_BUILD
		COMMAND windeployqt $<TARGET_FILE:${TARGET}> --force)

	install(TARGETS ${TARGET} RUNTIME DESTINATION bin)
	install(DIRECTORY $<TARGET_FILE_DIR:${TARGET}>/ DESTINATION bin
		FILES_MATCHING PATTERN *.dll)
	install(DIRECTORY $<TARGET_FILE_DIR:${TARGET}>/ DESTINATION bin
		FILES_MATCHING PATTERN *.qm)

	set_property(INSTALL "bin/fips.exe"
		PROPERTY CPACK_START_MENU_SHORTCUTS "Fips"
	)

	set(CPACK_GENERATOR WIX)
	set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE.txt)
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "Fips-3")
	set(CPACK_WIX_PATCH_FILE ${RESOURCES_DIR}/SupportedOS.wixpatch)
	set(CPACK_WIX_UPGRADE_GUID C61E8C2B-9E0C-45D4-AFAF-388E4F128FCF)
	set(CPACK_WIX_PRODUCT_ICON ${RESOURCES_DIR}/fips.ico)
	set(CPACK_WIX_PROGRAM_MENU_FOLDER ".")
	include(CPack)
elseif(UNIX)
	set(XDG_PREFIX "${CMAKE_INSTALL_PREFIX}/share")
	set(XDG_DESKTOP_DIR "${XDG_PREFIX}/applications")
	set(XDG_METAINFO_DIR "${XDG_PREFIX}/metainfo")
	set(XDG_ICONS_DIR "${XDG_PREFIX}/icons")

	configure_file("dist/freedesktop/space.fips.Fips.desktop.in" "space.fips.Fips.desktop" @ONLY)

	install(TARGETS "${TARGET}" DESTINATION bin)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/space.fips.Fips.desktop" DESTINATION ${XDG_DESKTOP_DIR})
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/space.fips.Fips.metainfo.xml" DESTINATION ${XDG_METAINFO_DIR})
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/scalable/space.fips.Fips.svg" DESTINATION "${XDG_ICONS_DIR}/hicolor/scalable/apps")
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/128x128/space.fips.Fips.png" DESTINATION "${XDG_ICONS_DIR}/hicolor/128x128/apps")
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/256x256/space.fips.Fips.png" DESTINATION "${XDG_ICONS_DIR}/hicolor/256x256/apps")
endif(APPLE)

enable_testing()
add_executable(test_fits test/fits.cpp src/fits.cpp src/utils/exception.cpp src/abstractfitsstorage.cpp src/mmapfitsstorage.cpp)
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

add_executable(test_opengltransform test/opengltransform.cpp src/abstractopengltransform.cpp include/abstractopengltransform.h src/opengltransform.cpp include/opengltransform.h src/openglplane.cpp include/openglplane.h)
target_link_libraries(test_opengltransform Qt5::Test Qt5::Gui)
add_test(test_opengltransform test_opengltransform)

add_executable(test_utils_minmax test/utils/minmax.cpp)
target_link_libraries(test_utils_minmax Qt5::Test)
add_test(test_utils_minmax test_utils_minmax)

add_executable(test_utils_openglrowalign test/utils/openglrowalign.cpp src/utils/openglrowalign.cpp)
target_link_libraries(test_utils_openglrowalign Qt5::Test)
add_test(test_utils_openglrowalign test_utils_openglrowalign)

add_executable(test_utils_swapbytes test/utils/swapbytes.cpp)
target_link_libraries(test_utils_swapbytes Qt5::Test)
add_test(test_utils_swapbytes test_utils_swapbytes)

add_executable(test_utils_optional test/utils/optional.cpp src/utils/exception.cpp)
target_link_libraries(test_utils_optional Qt5::Test)
add_test(test_utils_optional test_utils_optional)
