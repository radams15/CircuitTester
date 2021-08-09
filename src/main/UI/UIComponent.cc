//
// Created by rhys on 15/05/2021.
//

#include <iostream>
#include "UIComponent.h"

UIComponent::UIComponent(int id, std::string resourcePath): ID(id), SceneItem(std::move(resourcePath)){
    settingsBox = new QVBoxLayout();
}

bool UIComponent::equals(UIComponent* c) {
    return this == c;
}