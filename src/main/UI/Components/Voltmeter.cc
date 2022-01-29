//
// Created by rhys on 08/04/2021.
//

#include "Voltmeter.h"

Voltmeter::Voltmeter() : ResistiveElement(ID, ":/images/voltmeter.png") {
    // Add spacer at bottom to push all widgets up to top.
    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Voltmeter::getResistance() {
    // Max resistance means minimum current, which is the perfect voltmeter.
    return 10000000;
}
