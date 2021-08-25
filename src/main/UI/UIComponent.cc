//
// Created by rhys on 15/05/2021.
//

#include <iostream>
#include "UIComponent.h"

int UIComponent::currentId = 0;

UIComponent::UIComponent(int id, std::string resourcePath): ID(id), SceneItem(std::move(resourcePath)){
    componentId = currentId;
    currentId++;

    std::cout << "New component: " << componentId << std::endl;

    settingsBox = new QVBoxLayout();
}

bool UIComponent::equals(UIComponent* c) {
    return this == c;
}