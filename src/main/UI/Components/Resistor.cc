//
// Created by rhys on 08/04/2021.
//

#include <QLabel>
#include "Resistor.h"

Resistor::Resistor(double resistance) : ResistiveElement(ID, ":/images/resistor.png") {
    // Box to hold the resistance spinner and label.
    auto* resistanceBox = new QHBoxLayout();
    auto* resistanceLabel = new QLabel("Resistance (Ω)");
    resistanceSpinner = new QDoubleSpinBox;
    // Set minimum to 0.1 Ohm, maximum to the maximum size of a double.
    resistanceSpinner->setMinimum(0.1);
    resistanceSpinner->setMaximum(DBL_MAX);
    resistanceBox->addWidget(resistanceLabel);
    resistanceBox->addWidget(resistanceSpinner);
    settingsBox->addLayout(resistanceBox);

    // Validate voltage to ensure that it is greater than 0.1 Ohm. If it is less than 0.1 Ohm set voltage to 0.1 Ohm.
    resistanceSpinner->setValue(resistance < 0.1? 0.1:resistance);

    // Add spacer at bottom to push all widgets up to top.
    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Resistor::getResistance() {
    return resistanceSpinner->value();
}

YAML::Node Resistor::toYaml(){
    YAML::Node out;

    // Get the type of component.
    out["type"] = getId();


    out["resistance"] = getResistance();


    // Add the coordinates of the component in a list of [x,y].
	std::vector<double> position;
	position.push_back(pos().x());
	position.push_back(pos().y());
    out["pos"] = position;

    return out;
}
