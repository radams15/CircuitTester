//
// Created by rhys on 08/04/2021.
//

#include <QLabel>
#include "Voltmeter.h"

Voltmeter::Voltmeter() : ResistiveElement(ID, ":/images/voltmeter.png") {
    // Add spacer at bottom to push all widgets up to top.
    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Voltmeter::getResistance() {
    // Max resistance means minimum current, which is the perfect voltmeter.
    return 10000000;
}

YAML::Node Voltmeter::toYaml(){
    YAML::Node out;

    // Get the type of component.
    out["type"] = getId();

    // Add the coordinates of the component in a list of [x,y].
	std::vector<double> position;
	position.push_back(pos().x());
	position.push_back(pos().y());
    out["pos"] = position;

    return out;
}
