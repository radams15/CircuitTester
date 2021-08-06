//
// Created by rhys on 21/04/2021.
//

#ifndef LAYOUTTEST1_BATTERY_H
#define LAYOUTTEST1_BATTERY_H


#include <QDoubleSpinBox>
#include "../UIComponent.h"

class Battery : public UIComponent {
private:

    QDoubleSpinBox* voltageSpinner;

public:
    enum{
        ID = UI_BATTERY
    };

    double getVoltage();

    Battery();
};


#endif //LAYOUTTEST1_BATTERY_H
