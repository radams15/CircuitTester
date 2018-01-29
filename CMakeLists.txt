cmake_minimum_required(VERSION 3.5)

project(CircuitTester LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES DEBUG) # if debugging, build tests
    message("Tests Enabled")

    add_subdirectory(lib/doctest)
    add_subdirectory(src/test)

else()
    message("Tests Disabled!")
endif()

add_subdirectory(src/main)