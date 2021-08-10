//
// Created by rhys on 10/08/2021.
//

#include <QLabel>
#include "Switch.h"

Switch::Switch() : UIComponent(ID, ":/images/switch_open.png") {
    auto* enabledLabel = new QLabel("Enabled");
    enabledButton = new QCheckBox;
    enabledButton->setChecked(true);

    auto* enabledBox = new QHBoxLayout();
    enabledBox->addWidget(enabledLabel);
    enabledBox->addWidget(enabledButton);
    settingsBox->addLayout(enabledBox);
}

bool Switch::getEnabled() {
    return enabledButton->isChecked();
}
