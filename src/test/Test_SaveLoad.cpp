/** @file Test_SaveLoad.cc
 * This tests the saving/loading functions/
 * @date 12/08/2021
 * @author Rhys Adams
 */

#include <gtest/gtest.h>
#include <QMainWindow>
#include <QApplication>
#include <fstream>

#define private public

#include "../main/Saves/CircuitSaver.h"
#include "../main/UI/Components/Battery.h"
#include "../main/UI/Components/Resistor.h"

/** @brief Serialise #1.
 * Tests to see whether a simple circuit is serialised properly.
 *
 */
TEST(SaveLoad, Serialise1){
    int ret = 0;
    QApplication a(ret, nullptr);

    Scene s;
    s.setSceneRect(QRectF(0, 0, 5000, 5000));

    std::vector<UIComponent*> components = {
            new Battery(),
            new Resistor()
    };

    s.addItem(components[0]);
    s.addItem(components[1]);

    components[0]->setPos(10, 20);
    components[1]->setPos(1, 17);

    ((Battery*)components[0])->voltageSpinner->setValue(15);
    ((Resistor*)components[1])->resistanceSpinner->setValue(2);
    std::vector<Line*> arrows = {
            new Line(components[0], components[1]),
            new Line(components[1], components[0]),
    };
    components[0]->addArrow(arrows[0]);
    components[0]->addArrow(arrows[1]);
    components[1]->addArrow(arrows[0]);
    components[1]->addArrow(arrows[1]);

    std::string out = CircuitSaver::serialiseCircuit("Circuit 1", SceneItems{components, arrows, nullptr});

    /*std::string expected = R"({"name":"Circuit 1","parts":[{"component":{"state":true,"type":2,"voltage":15.0},"connections":[[1.0,17.0]]},{"component":{"resistance":2.0,"type":1},"connections":[[10.0,20.0]]}]})";

    ASSERT_EQ(out, expected);*/
}