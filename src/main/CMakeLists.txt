if(APPLE)
    set(SYS "macos")
elseif(UNIX)
    set(SYS "unix")
elseif(WIN32)
    set(SYS "windows")
else()
    set(SYS "unknown")
endif()

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
        UI/UIComponent.cc
        UI/UIComponent.h
        UI/AnalysisMapper.cc
        UI/AnalysisMapper.h
        Saves/FileUtils.cc
        Saves/FileUtils.h
	Saves/CircuitSaver.cc
	Saves/CircuitSaver.h
	UI/Components/ResistiveElement.h
)

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
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib/MNA")

find_package(Eigen3 3.3.9 REQUIRED)
find_package(PkgConfig)
find_path(MNA_INC "libMNA.h")
include_directories(${EIGEN3_INCLUDE_DIR} ${CIRCUIT_INCLUDE_DIRS} ${MNA_INC})

target_link_libraries(CircuitTester PRIVATE Qt${QT_VER}::Widgets nlohmann_json::nlohmann_json "${CMAKE_SOURCE_DIR}/lib/MNA/libMNA.so")

target_compile_options(CircuitTester PUBLIC)


set(BUILD_DIR ${PROJECT_SOURCE_DIR}/builds/${SYS})

install(TARGETS CircuitTester
        RUNTIME DESTINATION bin)
