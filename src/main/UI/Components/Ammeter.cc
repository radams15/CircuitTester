//
// Created by rhys on 08/04/2021.
//

#include "Ammeter.h"

Ammeter::Ammeter() : ResistiveElement(ID, ":/images/ammeter.png") {
    // Add spacer at bottom to push all widgets up to top.
    settingsBox->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding,QSizePolicy::Expanding));
}

double Ammeter::getResistance() {
    // Min resistance means max current, which is the perfect ammeter.
    return 0.000000001;
}

json::jobject Ammeter::toJson(){
    json::jobject out;

    // Get the type of component.
    out["type"] = getId();

    // Add the coordinates of the component in a list of [x,y].
	std::vector<double> position;
	position.push_back(pos().x());
	position.push_back(pos().y());
    out["pos"] = position;

    return out;
}
