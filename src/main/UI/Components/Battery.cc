//
// Created by rhys on 21/04/2021.
//

#include <iostream>
#include <QLabel>
#include "Battery.h"

Battery::Battery(double voltage, bool on) : UIComponent(ID, ":/images/battery.png") {
    // Box to hold voltage spinner and label
    auto* voltageBox = new QHBoxLayout;
    auto* voltageLabel = new QLabel("Voltage (V)");
    voltageSpinner = new QDoubleSpinBox;
    // Set minimum to 0.1V, maximum to the maximum size of a double.
    voltageSpinner->setMinimum(0.1);
    voltageSpinner->setMaximum(DBL_MAX);
    // Add components to the voltage box.
    voltageBox->addWidget(voltageLabel);
    voltageBox->addWidget(voltageSpinner);
    // Add the voltage box to the main settings layout.
    settingsBox->addLayout(voltageBox);

    // Box to hold on/off button and label
    auto* onOffBox = new QHBoxLayout;
    auto* onOffLabel = new QLabel("On/Off");
    onOffCheckbox = new QCheckBox;
    // Add components to on/off box.
    onOffBox->addWidget(onOffLabel);
    onOffBox->addWidget(onOffCheckbox);
    // Add on/off box to the main settings layout.
    settingsBox->addLayout(onOffBox);

    // Validate voltage to ensure that it is greater than 0.1V. If it is less than 0.1V set voltage to 0.1V.
    voltageSpinner->setValue(voltage < 0.1? 0.1 : voltage);
    // Can't validate a bool.
    onOffCheckbox->setChecked(on);

    // Add spacer at bottom to push all widgets up to top.
    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Battery::getVoltage() {
    // If battery is on return the voltage, otherwise return 0.0001, which is almost zero
    // but cannot be 0 otherwise the MNA will not function.
    return onOffCheckbox->isChecked()? voltageSpinner->value() : 0.0001;
}

YAML::Node Battery::toYaml(){
    YAML::Node out;

    // Get the type of component.
    out["type"] = getId();


    out["voltage"] = getVoltageValue();
    out["state"] = getState();


    // Add the coordinates of the component in a list of [x,y].
	std::vector<double> position;
	position.push_back(pos().x());
	position.push_back(pos().y());
    out["pos"] = position;

    return out;
}
