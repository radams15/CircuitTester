//
// Created by rhys on 21/04/2021.
//

#include <iostream>
#include "Battery.h"

double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Battery::Battery() : UIComponent(ID, ":/images/battery.png") {
    voltageSpinner = new QDoubleSpinBox;
    settingsBox->addWidget(voltageSpinner);

    voltageSpinner->setValue(fRand(0.1, 100));
}

double Battery::getVoltage() {
    return voltageSpinner->value();
}
