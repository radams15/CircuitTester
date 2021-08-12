//
// Created by rhys on 12/08/2021.
//

#ifndef CIRCUITTESTER_RESISTIVEELEMENT_H
#define CIRCUITTESTER_RESISTIVEELEMENT_H

#include <utility>

#include "../UIComponent.h"

class ResistiveElement : public UIComponent {
private:

public:
    ResistiveElement(int ID, std::string resourcePath) : UIComponent(ID, std::move(resourcePath)){}

    virtual double getResistance() = 0;
};

#endif //CIRCUITTESTER_RESISTIVEELEMENT_H
