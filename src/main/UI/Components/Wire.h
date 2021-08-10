//
// Created by rhys on 10/08/2021.
//

#ifndef LAYOUTTEST1_WIRE_H
#define LAYOUTTEST1_WIRE_H


#include <map>
#include <string>

#include <QDoubleSpinBox>
#include <QComboBox>
#include "../UIComponent.h"

class Wire : public UIComponent {
private:
    std::map<std::string, long double> resistivities = {
            {"Copper", 0.0000000168}
    };

public:
    enum{
        ID = UI_WIRE
    };

    QDoubleSpinBox* lengthSpinner;
    QDoubleSpinBox* areaSpinner;
    QComboBox* wireCombo;

    Wire();

    double getResistance();
};


#endif //LAYOUTTEST1_RESISTOR_H
