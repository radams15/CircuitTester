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

set(BUILD_DIR ${PROJECT_SOURCE_DIR}/builds/${SYS})

install(TARGETS CircuitTester
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${BUILD_DIR}/debug/bin)

install(TARGETS CircuitTester
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${BUILD_DIR}/release/bin)


add_custom_target(copy_libs
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll         ${BUILD_DIR}/debug/bin
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll          ${BUILD_DIR}/debug/bin
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll      ${BUILD_DIR}/debug/bin

  # Output Message
  COMMENT "Copying Qt binaries from '${Qt5Core_DIR}/../../bin/' to '${CMAKE_BINARY_DIR}'" VERBATIM
)
