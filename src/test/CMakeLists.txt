set(TEST_FILES
        nodal_analysis.cpp)

set(C_FILES
        ../main/Analysis/Equation.cpp
        ../main/Analysis/Equation.h
        ../main/Analysis/MNACircuit.cpp
        ../main/Analysis/MNACircuit.h
        ../main/Analysis/MNAElement.cpp
        ../main/Analysis/MNAElement.h
        ../main/Analysis/MNASolution.cpp
        ../main/Analysis/MNASolution.h
        ../main/Analysis/MNA.h
        ../main/Components/Battery.cpp
        ../main/Components/Battery.h
        ../main/Components/Resistor.cpp
        ../main/Components/Resistor.h
        ../main/Components/CurrentSource.cpp
        ../main/Components/CurrentSource.h)

set(EIGEN3_ROOT ${CMAKE_SOURCE_DIR}/lib/eigen-3.3.9)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib)

find_package(Eigen3 3.3.9 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(test ${TEST_FILES} ${C_FILES})
target_link_libraries(test gtest_main)