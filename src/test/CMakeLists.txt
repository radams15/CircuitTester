set(TEST_FILES
        Test_MNA.cpp
        Test_MNACircuit.cpp)

set(C_FILES
        ../main/Analysis/Equation.cpp
        ../main/Analysis/Equation.h
        ../main/Analysis/MNACircuit.cpp
        ../main/Analysis/MNACircuit.h
        ../main/Analysis/MNAComponent.cpp
        ../main/Analysis/MNAComponent.h
        ../main/Analysis/MNASolution.cpp
        ../main/Analysis/MNASolution.h
        ../main/Analysis/MNA.h
        ../main/UI/Components/Battery.cpp
        ../main/UI/Components/Battery.h
        ../main/UI/Components/Resistor.cpp
        ../main/UI/Components/Resistor.h
        ../main/Analysis/Unknown.cpp ../main/Analysis/Unknown.h Test_MNACircuit.cpp ../main/Analysis/Term.cpp ../main/Analysis/Term.h)

set(EIGEN3_ROOT ${CMAKE_SOURCE_DIR}/lib/eigen)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(test ${TEST_FILES} ${C_FILES})
target_link_libraries(test gtest_main)