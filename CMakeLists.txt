cmake_minimum_required(VERSION 3.5)

project(CircuitTester LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(MAKE_DOCS ON)
set(DO_TESTS ON CACHE BOOL "Enable Testing")
set(MAKE_PROG ON CACHE BOOL "Enable building program")

set(DOCTEST_NO_INSTALL ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DO_TESTS OR MAKE_PROG)
    find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
endif()

if(DO_TESTS) # if debugging, build tests
    message("Tests Enabled")

    add_subdirectory(lib/googletest)
    add_subdirectory(src/test)

else()
    message("Tests Disabled!")
endif()

if(MAKE_DOCS)
    add_subdirectory(docs)
endif(MAKE_DOCS)

IF(MAKE_PROG)
    add_subdirectory(src/main)
endif(MAKE_PROG)
