//
// Created by rhys on 21/04/2021.
//

#ifndef LAYOUTTEST1_BATTERY_H
#define LAYOUTTEST1_BATTERY_H


#include <QDoubleSpinBox>
#include <QCheckBox>
#include "../UIComponent.h"

class Battery : public UIComponent {
public:
    QDoubleSpinBox* voltageSpinner;
    QCheckBox* onOffCheckbox;

    enum{
        ID = UI_BATTERY
    };

    double getVoltage();

    Battery();
};


#endif //LAYOUTTEST1_BATTERY_H
