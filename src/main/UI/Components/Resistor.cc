//
// Created by rhys on 08/04/2021.
//

#include <QLabel>
#include "Resistor.h"

Resistor::Resistor() : UIComponent(ID, ":/images/resistor.png") {
    auto* resistanceLabel = new QLabel("Resistance (Ω)");
    resistanceSpinner = new QDoubleSpinBox;
    resistanceSpinner->setMinimum(0.1);

    auto* resistanceBox = new QHBoxLayout();
    resistanceBox->addWidget(resistanceLabel);
    resistanceBox->addWidget(resistanceSpinner);
    settingsBox->addLayout(resistanceBox);

    resistanceSpinner->setValue(1.0f);

    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Resistor::getResistance() {
    return resistanceSpinner->value();
}
