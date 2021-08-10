//
// Created by rhys on 21/04/2021.
//

#include <iostream>
#include <QLabel>
#include "Battery.h"

Battery::Battery() : UIComponent(ID, ":/images/battery.png") {
    auto* voltageBox = new QHBoxLayout;
    auto* voltageLabel = new QLabel("Voltage (V)");
    voltageSpinner = new QDoubleSpinBox;
    voltageSpinner->setMinimum(0.1);
    voltageBox->addWidget(voltageLabel);
    voltageBox->addWidget(voltageSpinner);
    settingsBox->addLayout(voltageBox);

    auto* onOffBox = new QHBoxLayout;
    auto* onOffLabel = new QLabel("On/Off");
    onOffCheckbox = new QCheckBox;
    onOffCheckbox->setChecked(true);
    onOffBox->addWidget(onOffLabel);
    onOffBox->addWidget(onOffCheckbox);
    settingsBox->addLayout(onOffBox);

    voltageSpinner->setValue(1.0f);

    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Battery::getVoltage() {
    return onOffCheckbox->isChecked()? voltageSpinner->value() : 0.0001;
}
