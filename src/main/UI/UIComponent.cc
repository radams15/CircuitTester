//
// Created by rhys on 15/05/2021.
//

#include <iostream>
#include "UIComponent.h"

int UIComponent::currentId = 0;

UIComponent::UIComponent(int id, std::string resourcePath): ID(id), SceneItem(resourcePath) {
    // Set component id to the current value of the UIComponent ID.
    componentId = currentId;

    // Increment currentId to allow the next component a different ID.
    currentId++;

    // Create the settings box for class derivatives to use.
    settingsBox = new QVBoxLayout();

    onColor = QColor(0,255,0);
    offColor = QColor(0,0,0);
}

void UIComponent::setState(bool on) {
    if(on){
        setColour(onColor);
    }else{
        setColour(offColor);
    }
}
