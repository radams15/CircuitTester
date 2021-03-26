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
        mainwindow.cpp
        mainwindow.h
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

target_link_libraries(CircuitTester PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS CircuitTester
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/builds/${SYS}/debug/bin)

install(TARGETS CircuitTester
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/builds/${SYS}/release/bin)