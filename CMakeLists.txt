cmake_minimum_required(VERSION 2.8)

project(CircuitTester LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(MAKE_DOCS ON)
set(DO_TESTS ON CACHE BOOL "Enable Testing")
set(MAKE_PROG ON CACHE BOOL "Enable building program")

set(DOCTEST_NO_INSTALL ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${QT_VER})
else()
    set(QT_VER 5)
endif()

message("QT version: ${QT_VER}")

if(DO_TESTS OR MAKE_PROG)
    add_subdirectory(lib/nlohmann_json_cmake_fetchcontent)
    add_subdirectory(resources)
    find_package(QT NAMES Qt${QT_VER} COMPONENTS Widgets REQUIRED)
    find_package(Qt${QT_VER} COMPONENTS Widgets REQUIRED)
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
