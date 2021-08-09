//
// Created by rhys on 15/05/2021.
//

#ifndef CIRCUITTESTER_UICOMPONENT_H
#define CIRCUITTESTER_UICOMPONENT_H

#include <string>

#include <utility>
#include <QVBoxLayout>

#include "SceneItem.h"

enum ComponentType{
    UI_RESISTOR,
    UI_BATTERY,
};

class UIComponent : public SceneItem{
protected:
    int ID;

    UIComponent(int id, std::string resourcePath);

public:
    inline int getId() const { return ID; }

    QVBoxLayout* settingsBox;

    int n0 = -1;
    int n1 = -1;

    std::vector<Arrow*> connections;

    bool equals(UIComponent* c);
};


#endif //CIRCUITTESTER_UICOMPONENT_H
