cmake_minimum_required(VERSION 3.16)

project(fips LANGUAGES C CXX VERSION 3.4.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

find_package(Qt6 REQUIRED COMPONENTS
	Core
	Gui
	OpenGL
	OpenGLWidgets
	Test
	Widgets)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)
qt_add_executable(fips ${SOURCES})
target_link_libraries(fips PRIVATE Qt6::Widgets Qt6::OpenGLWidgets Qt6::OpenGL)
target_compile_definitions(fips PUBLIC PROJECT_VERSION="${PROJECT_VERSION}")
set_target_properties(fips PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON)

set(ICON_NAME fips)

if (UNIX)
	set(XDG_PREFIX "${CMAKE_INSTALL_PREFIX}/share")
	set(XDG_DESKTOP_DIR "${XDG_PREFIX}/applications")
	set(XDG_METAINFO_DIR "${XDG_PREFIX}/metainfo")
	set(XDG_ICONS_DIR "${XDG_PREFIX}/icons")

	configure_file("dist/freedesktop/space.fips.Fips.desktop.in" "space.fips.Fips.desktop" @ONLY)

	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/space.fips.Fips.desktop" DESTINATION ${XDG_DESKTOP_DIR})
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/space.fips.Fips.metainfo.xml" DESTINATION ${XDG_METAINFO_DIR})
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/scalable/space.fips.Fips.svg" DESTINATION "${XDG_ICONS_DIR}/hicolor/scalable/apps")
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/128x128/space.fips.Fips.png" DESTINATION "${XDG_ICONS_DIR}/hicolor/128x128/apps")
	install(FILES "${PROJECT_SOURCE_DIR}/dist/freedesktop/256x256/space.fips.Fips.png" DESTINATION "${XDG_ICONS_DIR}/hicolor/256x256/apps")
endif()

if(WIN32)
	set(RESOURCES_DIR "${PROJECT_SOURCE_DIR}/dist/windows")
	set(ICON_FILE "${RESOURCES_DIR}/${ICON_NAME}.ico")

	if (NOT PROJECT_VERSION_TWEAK)
		set(PROJECT_VERSION_TWEAK 0)
	endif()

	configure_file(${RESOURCES_DIR}/metafile.rc.in metafile.rc)

	set_property(TARGET fips APPEND PROPERTY SOURCES metafile.rc)
endif()

install(TARGETS fips
	BUNDLE  DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

qt_generate_deploy_app_script(
	TARGET fips
	OUTPUT_SCRIPT deploy_script
	NO_UNSUPPORTED_PLATFORM_ERROR)

install(SCRIPT ${deploy_script})

enable_testing()

add_executable(test_fits test/fits.cpp src/fits.cpp src/utils/exception.cpp src/abstractfitsstorage.cpp src/mmapfitsstorage.cpp)
target_compile_definitions(test_fits PUBLIC PROJECT_ROOT="${PROJECT_SOURCE_DIR}")
target_link_libraries(test_fits Qt6::Test)
add_test(test_fits test_fits)

add_executable(test_openglshaderuniforms test/openglshaderuniforms.cpp src/openglshaderunifroms.cpp)
target_link_libraries(test_openglshaderuniforms Qt6::Test)
add_test(test_openglshaderuniforms test_openglshaderuniforms)

add_executable(test_opengltransform test/opengltransform.cpp src/abstractopengltransform.cpp include/abstractopengltransform.h src/opengltransform.cpp include/opengltransform.h src/openglplane.cpp include/openglplane.h)
target_link_libraries(test_opengltransform Qt6::Test Qt6::Gui Qt6::OpenGL)
add_test(test_opengltransform test_opengltransform)

add_executable(test_utils_minmax test/utils/minmax.cpp)
target_link_libraries(test_utils_minmax Qt6::Test)
add_test(test_utils_minmax test_utils_minmax)

add_executable(test_utils_openglrowalign test/utils/openglrowalign.cpp src/utils/openglrowalign.cpp)
target_link_libraries(test_utils_openglrowalign Qt6::Test)
add_test(test_utils_openglrowalign test_utils_openglrowalign)

add_executable(test_utils_swapbytes test/utils/swapbytes.cpp)
target_link_libraries(test_utils_swapbytes Qt6::Test)
add_test(test_utils_swapbytes test_utils_swapbytes)

add_executable(test_utils_optional test/utils/optional.cpp src/utils/exception.cpp)
target_link_libraries(test_utils_optional Qt6::Test)
add_test(test_utils_optional test_utils_optional)
