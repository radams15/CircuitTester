set(TEST_FILES
        Test_MNA.cc
        Test_MNACircuit.cc Test_SaveLoad.cpp)

set(C_FILES
        ../../resources/resources.qrc

       ../main/UI/MainWindow.cc
       ../main/UI/MainWindow.h
       ../main/UI/Arrow.cc
       ../main/UI/Arrow.h
       ../main/UI/Scene.cc
       ../main/UI/Scene.h
       ../main/UI/SceneItem.cc
       ../main/UI/SceneItem.h
       ../main/UI/SceneText.cc
       ../main/UI/SceneText.h
       ../main/UI/SettingsMenu.cc
       ../main/UI/SettingsMenu.h

       ../main/Analysis/Equation.cc
       ../main/Analysis/Equation.h
       ../main/Analysis/MNACircuit.cc
       ../main/Analysis/MNACircuit.h
       ../main/Analysis/MNAComponent.cc
       ../main/Analysis/MNAComponent.h
       ../main/Analysis/MNASolution.cc
       ../main/Analysis/MNASolution.h
       ../main/Analysis/MNA.h
       ../main/UI/Components/Battery.cc
       ../main/UI/Components/Battery.h
       ../main/UI/Components/Resistor.cc
       ../main/UI/Components/Resistor.h
       ../main/UI/Components/Wire.cc
       ../main/UI/Components/Wire.h
       ../main/UI/Components/Switch.cc
       ../main/UI/Components/Switch.h
       ../main/Analysis/Unknown.cc
       ../main/Analysis/Unknown.h
       ../main/Analysis/Term.cc
       ../main/Analysis/Term.h
       ../main/UI/UIComponent.cc
       ../main/UI/UIComponent.h
       ../main/UI/AnalysisMapper.cc
       ../main/UI/AnalysisMapper.h
        ../main/Saves/UserUtils.cpp
        ../main/Saves/UserUtils.h ../main/Saves/CircuitSaver.cc ../main/Saves/CircuitSaver.h ../main/UI/Components/ResistiveElement.h)

set(EIGEN3_ROOT ${CMAKE_SOURCE_DIR}/lib/eigen)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


add_executable(test ${TEST_FILES} ${C_FILES})
target_link_libraries(test gtest_main Qt${QT_VERSION_MAJOR}::Widgets nlohmann_json::nlohmann_json)
