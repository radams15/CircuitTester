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
        main.cpp
        UI/mainwindow.cpp
        UI/mainwindow.h
        Analysis/Equation.cpp
        Analysis/Equation.h
        Analysis/MNACircuit.cpp
        Analysis/MNACircuit.h
        Analysis/MNAElement.cpp
        Analysis/MNAElement.h
        Analysis/MNASolution.cpp
        Analysis/MNASolution.h
        Analysis/MNA.h
        Components/Battery.cpp Components/Battery.h Components/Resistor.cpp Components/Resistor.h Components/CurrentSource.cpp Components/CurrentSource.h)

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

set(EIGEN3_ROOT ${CMAKE_SOURCE_DIR}/lib/eigen-3.3.9)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib)

find_package(Eigen3 3.3.9 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

target_link_libraries(CircuitTester PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set(BUILD_DIR ${PROJECT_SOURCE_DIR}/builds/${SYS})

install(TARGETS CircuitTester
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${BUILD_DIR}/debug/bin)

install(TARGETS CircuitTester
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${BUILD_DIR}/release/bin)


add_custom_target(copy_libs
        COMMAND windeployqt ${BUILD_DIR}/release/bin
        # Output Message
        COMMENT "Copying Qt binaries from '${Qt5Core_DIR}/../../bin/' to '${CMAKE_BINARY_DIR}'" VERBATIM
)
