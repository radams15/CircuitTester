cmake_minimum_required(VERSION 2.8)

set(CMAKE_PROJECT_VERSION 2.0.0)
set(CMAKE_PROJECT_VERSION_MAJOR 2)
set(CMAKE_PROJECT_VERSION_MINOR 0)
set(CMAKE_PROJECT_VERSION_PATCH 0)

project(CircuitTester LANGUAGES CXX)
enable_language(CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(DO_TESTS ON CACHE BOOL "Enable Testing")
set(MAKE_PROG ON CACHE BOOL "Enable building program")

set(DOCTEST_NO_INSTALL ON)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(DO_TESTS OR MAKE_PROG)
    add_subdirectory(resources)

    find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
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
