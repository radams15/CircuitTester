//
// Created by rhys on 10/08/2021.
//

#include <QLabel>
#include "Switch.h"

Switch::Switch(bool on) : ResistiveElement(ID, ":/images/switch_open.png") {
    // Box to hold on/off label and switch.
    auto* enabledBox = new QHBoxLayout();
    auto* enabledLabel = new QLabel("Enabled");
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


json::jobject Switch::toJson(){
    json::jobject out;

    // Get the type of component.
    out["type"] = getId();

    out["state"] = getState();

    // Add the coordinates of the component in a list of [x,y].
	std::vector<double> position;
	position.push_back(pos().x());
	position.push_back(pos().y());
    out["pos"] = position;

    return out;
}
