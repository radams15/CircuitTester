if(APPLE)
    set(SYS "macos")
elseif(UNIX)
    set(SYS "unix")
elseif(WIN32)
    set(SYS "windows")
else()
    set(SYS "unknown")
endif()

include(KDEInstallDirs)
include(KDECMakeSettings)
include(FeatureSummary)

set(PROJECT_SOURCES
        main.cc

        ${CMAKE_SOURCE_DIR}/resources/resources.qrc

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
        Saves/FileUtils.h Saves/CircuitSaver.cc Saves/CircuitSaver.h UI/Components/ResistiveElement.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CircuitTester
            ${PROJECT_SOURCES}
            )
else()
    if(ANDROID)
        add_library(CircuitTester SHARED
                ${PROJECT_SOURCES}
                )
    else()
        add_executable(CircuitTester
                ${PROJECT_SOURCES}
                )
    endif()
endif()

set(EIGEN3_ROOT ${CMAKE_SOURCE_DIR}/lib/eigen)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib)

find_package(Eigen3 3.3.9 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

target_link_libraries(CircuitTester PRIVATE
        Qt${QT_VER}::Widgets
        KF5::CoreAddons
        KF5::I18n
        KF5::XmlGui
        nlohmann_json::nlohmann_json)


install(TARGETS CircuitTester ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
