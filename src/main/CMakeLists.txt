if(APPLE)
    set(SYS "macos")
elseif(UNIX)
    set(SYS "unix")
elseif(WIN32)
    set(SYS "windows")
else()
    set(SYS "unknown")
endif()

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(PROJECT_SOURCES
        main.cc

        ../../resources/resources.qrc

        UI/MainWindow.cc
        UI/MainWindow.h
        UI/Line.cc
        UI/Line.h
        UI/Scene.cc
        UI/Scene.h
        UI/SceneItem.cc
        UI/SceneItem.h
        UI/SceneText.cc
        UI/SceneText.h
        UI/SettingsMenu.cc
        UI/SettingsMenu.h

        Analysis/Equation.cc
        Analysis/Equation.h
        Analysis/Circuit.cc
        Analysis/Circuit.h
        Analysis/Component.cc
        Analysis/Component.h
        Analysis/Solution.cc
        Analysis/Solution.h
        UI/Components/Battery.cc
        UI/Components/Battery.h
        UI/Components/Resistor.cc
        UI/Components/Resistor.h
        UI/Components/Wire.cc
        UI/Components/Wire.h
        UI/Components/Switch.cc
        UI/Components/Switch.h
        UI/Components/Voltmeter.cc
        UI/Components/Voltmeter.h
        UI/Components/Ammeter.cc
        UI/Components/Ammeter.h
        Analysis/Unknown.cc
        Analysis/Unknown.h
        Analysis/Term.cc
        Analysis/Term.h
        UI/UIComponent.cc
        UI/UIComponent.h
        UI/AnalysisMapper.cc
        UI/AnalysisMapper.h
        Saves/FileUtils.cc
        Saves/FileUtils.h Saves/json.cpp Saves/json.cpp Saves/CircuitSaver.cc Saves/CircuitSaver.h UI/Components/ResistiveElement.h UI/ExpandingSpacer.h UI/HamburgerMenu.cc UI/HamburgerMenu.h)

set(ICON_PATH ${PROJECT_SOURCE_DIR}/resources/CircuitTester.icns)
set(CircuitTester_ICON "${ICON_PATH}")

if(ANDROID)
    add_library(CircuitTester SHARED
            ${PROJECT_SOURCES}
            )
else()
    add_executable(CircuitTester
        MACOSX_BUNDLE
	    ${ICON_PATH}
            ${PROJECT_SOURCES}
            )
endif()

if(${APPLE})
	set_target_properties(CircuitTester PROPERTIES
	    BUNDLE True
	    MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifierCircuitTester
	    MACOSX_BUNDLE_BUNDLE_NAME CircuitTester
	    MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}"
	    MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
	    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/resources/template.plist.in
	    MACOSX_BUNDLE_ICON_FILE "CircuitTester.icns"
	)

	set_source_files_properties(${CircuitTester_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	file(COPY ${ICON_PATH} DESTINATION "CircuitTester.app/Contents/Resources")
endif()

set(EIGEN3_ROOT ${CMAKE_SOURCE_DIR}/lib/eigen)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib)

find_package(Eigen3 3.0.0 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

target_link_libraries(CircuitTester PRIVATE Qt4::QtGui)


set(BUILD_DIR ${PROJECT_SOURCE_DIR}/builds/${SYS})
