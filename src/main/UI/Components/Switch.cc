//
// Created by rhys on 10/08/2021.
//

#include <QtGui/QLabel>
#include "Switch.h"

Switch::Switch(bool on) : ResistiveElement(ID, ":/images/switch_open.png") {
    // Box to hold on/off label and switch.
    QHBoxLayout* enabledBox = new QHBoxLayout();
    QLabel* enabledLabel = new QLabel("Enabled");
    onOffCheckbox = new QCheckBox;
    // Add widgets to the box.
    enabledBox->addWidget(enabledLabel);
    enabledBox->addWidget(onOffCheckbox);
    // Add the box to the main settings layout.
    settingsBox->addLayout(enabledBox);

    // Set the state, can't validate bool.
    onOffCheckbox->setChecked(on);

    // Add spacer to push widget to top.
    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Switch::getResistance() {
    // Return the resistance relating to the state of the switch, high resistance for off, low resistance for on.
    return onOffCheckbox->isChecked() ? switchOn : switchOff;
}
