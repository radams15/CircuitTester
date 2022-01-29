//
// Created by rhys on 08/04/2021.
//

#include <QtGui/QLabel>
#include "Resistor.h"

Resistor::Resistor(double resistance) : ResistiveElement(ID, ":/images/resistor.png") {
    // Box to hold the resistance spinner and label.
    QHBoxLayout* resistanceBox = new QHBoxLayout();
    QLabel* resistanceLabel = new QLabel("Resistance (Ω)");
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
