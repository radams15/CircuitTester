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