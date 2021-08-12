//
// Created by rhys on 10/08/2021.
//

#ifndef LAYOUTTEST1_SWITCH_H
#define LAYOUTTEST1_SWITCH_H

#include <QDoubleSpinBox>
#include <QCheckBox>
#include "ResistiveElement.h"

class Switch : public ResistiveElement {
private:

    /** @brief The resistance when the switch is off is miniscule so that current can flow.
     *
     */
    const double switchOn = 0.000001;

    /** @brief The resistance when the switch is on is very large so that current cannot flow.
     *
     */
    const double switchOff = 10000000;

public:

    enum{
        ID = UI_SWITCH
    };

    QCheckBox* enabledButton;

    Switch();

    double getResistance() override;
};


#endif //LAYOUTTEST1_SWITCH_H