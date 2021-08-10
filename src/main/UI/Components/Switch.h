//
// Created by rhys on 10/08/2021.
//

#ifndef LAYOUTTEST1_SWITCH_H
#define LAYOUTTEST1_SWITCH_H

#include <QDoubleSpinBox>
#include <QCheckBox>
#include "../UIComponent.h"

class Switch : public UIComponent {
private:

public:
    enum{
        ID = UI_SWITCH
    };

    QCheckBox* enabledButton;

    Switch();

    bool getEnabled();
};


#endif //LAYOUTTEST1_RESISTOR_H
