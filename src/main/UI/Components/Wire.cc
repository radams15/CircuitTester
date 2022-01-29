//
// Created by rhys on 10/08/2021.
//

#include <QtGui/QLabel>
#include <iostream>
#include "Wire.h"

Wire::Wire(double length, double area, std::string material) : ResistiveElement(ID, ":/images/wire.png") {
    resistivities["Copper"] = 1.68E-8;
    resistivities["Iron"] = 1.68E-8;
    resistivities["Lead"] = 1.68E-8;
    resistivities["Carbon"] = 6E-4;

    // Box to hold length spinner and label
    QHBoxLayout* lengthBox = new QHBoxLayout();
    QLabel* lengthLabel = new QLabel("Length (cm)");
    lengthSpinner = new QDoubleSpinBox;
    // Set minimum to 0.1 cm, maximum to the maximum size of a double.
    lengthSpinner->setMinimum(0.1);
    lengthSpinner->setMaximum(DBL_MAX);
    lengthBox->addWidget(lengthLabel);
    lengthBox->addWidget(lengthSpinner);
    settingsBox->addLayout(lengthBox);

    // Box to hold area spinner and label
    QHBoxLayout* areaBox = new QHBoxLayout();
    QLabel* areaLabel = new QLabel("Area (mm^2)");
    areaSpinner = new QDoubleSpinBox;
    // Set minimum to 0.1 mm, maximum to the maximum size of a double.
    areaSpinner->setMinimum(0.1);
    areaSpinner->setMaximum(DBL_MAX);
    areaBox->addWidget(areaLabel);
    areaBox->addWidget(areaSpinner);
    settingsBox->addLayout(areaBox);

    // Box to hold material combobox and label.
    QHBoxLayout* wireBox = new QHBoxLayout();
    QLabel* wireLabel = new QLabel("Wire");
    wireCombo = new QComboBox;
    // Add each resistivity key (string) to the combobox.
    for(std::map<std::string,long double>::iterator it=resistivities.begin() ; it!=resistivities.end() ; it++){
        wireCombo->addItem(it->first.c_str());
    }
    wireBox->addWidget(wireLabel);
    wireBox->addWidget(wireCombo);
    settingsBox->addLayout(wireBox);

    // Validate length to ensure that it is greater than 0.1 cm. If it is less than 0.1 cm set voltage to 0.1 cm.
    lengthSpinner->setValue(length < 0.1? 0.1 : length);

    // Validate area to ensure that it is greater than 0.1 mm. If it is less than 0.1 mm set voltage to 0.1 mm.
    areaSpinner->setValue(area < 0.1? 0.1 : area);

    if(resistivities.find(material) != resistivities.end()) {
        // If the material is a valid one (in the resistivities map).
        //TODO Fix loading material from save -V
        //wireCombo->setCurrentText(QString::fromStdString(material));
    }else {
        // The material is invalid, set material to the first one in the combobox.
        wireCombo->setCurrentIndex(0);
    }

    // Add spacer to push widget to top.
    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Wire::getResistance() {
    // Get the length, convert cm to m by dividing by 100.
    long double length = lengthSpinner->value()/100L;

    // Get the area, convert mm^2 to m^2 by dividing by 1000^2
    long double area = areaSpinner->value()/1e6L;

    // Get the material by converting the wire combobox to text and then indexing the resistivities map.
    std::string material = wireCombo->currentText().toStdString();
    long double resistivity = resistivities[material];

    // Use the resistivity formula p=(RA/l), rearranged to R=(pl)/A
    long double resistance = (resistivity*length)/area;

    return (double) resistance;
}
