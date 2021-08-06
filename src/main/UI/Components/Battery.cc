//
// Created by rhys on 21/04/2021.
//

#include "Battery.h"

Battery::Battery() : UIComponent(ID, ":/images/battery.png") {
    voltageSpinner = new QDoubleSpinBox;
    settingsBox->addWidget(voltageSpinner);
}

double Battery::getVoltage() {
    return voltageSpinner->value();
}
