//
// Created by rhys on 08/04/2021.
//

#ifndef LAYOUTTEST1_RESISTOR_H
#define LAYOUTTEST1_RESISTOR_H

#include <QDoubleSpinBox>
#include "ResistiveElement.h"

class Resistor : public ResistiveElement {
private:

public:
    enum{
        ID = UI_RESISTOR
    };

    QDoubleSpinBox* resistanceSpinner;

    Resistor();

    double getResistance() override;
};


#endif //LAYOUTTEST1_RESISTOR_H
