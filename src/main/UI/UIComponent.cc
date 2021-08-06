//
// Created by rhys on 15/05/2021.
//

#include "UIComponent.h"

UIComponent::UIComponent(int id, std::string resourcePath): ID(id), SceneItem(std::move(resourcePath)){
    settingsBox = new QVBoxLayout;
}

std::string UIComponent::str() {
    return resourcePath + "(n" + std::to_string(n0) + ")";
}

bool UIComponent::equals(UIComponent* c) {
    return this == c;
}