//
// Created by rhys on 10/08/2021.
//

#ifndef LAYOUTTEST1_WIRE_H
#define LAYOUTTEST1_WIRE_H


#include <map>
#include <string>

#include <QDoubleSpinBox>
#include <QComboBox>
#include "ResistiveElement.h"

class Wire : public ResistiveElement {
private:
    std::map<std::string, long double> resistivities = {
            {"Copper", 1.68E-8},
            {"Iron", 1.0E-8},
            {"Lead", 2.2E-8},
            {"Carbon", 6E-4},
    };

public:
    enum{
        ID = UI_WIRE
    };

    QDoubleSpinBox* lengthSpinner;
    QDoubleSpinBox* areaSpinner;
    QComboBox* wireCombo;

    Wire();

    double getResistance() override;
};


#endif //LAYOUTTEST1_WIRE_H
