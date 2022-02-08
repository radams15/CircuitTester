set(TEST_FILES
        Test_MNA.cc
        Test_MNACircuit.cc
        Test_MNAComponent.cc
        Test_MNAEquation.cc)

set(C_FILES
        ../../resources/resources.qrc

        ../main/Analysis/Equation.cc
        ../main/Analysis/Equation.h
        ../main/Analysis/Circuit.cc
        ../main/Analysis/Circuit.h
        ../main/Analysis/Component.cc
        ../main/Analysis/Component.h
        ../main/Analysis/Solution.cc
        ../main/Analysis/Solution.h
       ../main/Analysis/Unknown.cc
       ../main/Analysis/Unknown.h
       ../main/Analysis/Term.cc
       ../main/Analysis/Term.h
)

set(EIGEN3_ROOT ${CMAKE_SOURCE_DIR}/lib/eigen)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


add_executable(test ${TEST_FILES} ${C_FILES})
target_link_libraries(test gtest_main Qt${QT_VERSION_MAJOR}::Widgets yaml-cpp)
