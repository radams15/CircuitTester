set(PROJECT_RESOURCES
        resources.qrc
        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CircuitTester
            ${PROJECT_RESOURCES}
            )
else()
    if(ANDROID)
        add_library(CircuitTester SHARED
                ${PROJECT_RESOURCES}
                )
    else()
        add_executable(CircuitTester
                ${PROJECT_RESOURCES}
                )
    endif()
endif()
