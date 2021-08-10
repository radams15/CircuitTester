//
// Created by rhys on 10/08/2021.
//

#include <QLabel>
#include <iostream>
#include "Wire.h"

Wire::Wire() : UIComponent(ID, ":/images/wire.png") {
    auto* lengthLabel = new QLabel("Length (cm)");
    lengthSpinner = new QDoubleSpinBox;
    lengthSpinner->setMinimum(0.1);

    auto* lengthBox = new QHBoxLayout();
    lengthBox->addWidget(lengthLabel);
    lengthBox->addWidget(lengthSpinner);

    auto* areaLabel = new QLabel("Area (mm^2)");
    areaSpinner = new QDoubleSpinBox;
    areaSpinner->setMinimum(0.1);

    auto* areaBox = new QHBoxLayout();
    areaBox->addWidget(areaLabel);
    areaBox->addWidget(areaSpinner);

    auto* wireLabel = new QLabel("Wire");
    wireCombo = new QComboBox;
    for(auto i : resistivities){
        wireCombo->addItem(i.first.c_str());
    }

    auto* wireBox = new QHBoxLayout();
    wireBox->addWidget(wireLabel);
    wireBox->addWidget(wireCombo);

    settingsBox->addLayout(lengthBox);
    settingsBox->addLayout(areaBox);
    settingsBox->addLayout(wireBox);

    lengthSpinner->setValue(1.0f);
    areaSpinner->setValue(1.0f);

    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Wire::getResistance() {
    double length = lengthSpinner->value()/100L;
    double area = areaSpinner->value()/1e6L;
    std::string material = wireCombo->currentText().toStdString();
    double resistivity = resistivities.at(material);

    double resistance = (resistivity*length)/area; // p=(RA/l), R=(pl)/A

    return resistance;
}