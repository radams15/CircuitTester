//
// Created by rhys on 09/05/2021.
//

#include "Unknown.h"

UnknownCurrent::UnknownCurrent(MNAElement *element) : Unknown(CURRENT) {
    this->element = element;
}

std::string UnknownCurrent::toTermName() {
    return "I"+std::to_string(element->n0)+"_"+std::to_string(element->n1);
}

bool UnknownCurrent::equals(Unknown* other) {
    if(other->t != Unknown::CURRENT){
        return false;
    }

    auto* c = (UnknownCurrent*) other;
    return c->element == element;
}

UnknownVoltage::UnknownVoltage(int node)  : Unknown(VOLTAGE) {
    this->node = node;
}

std::string UnknownVoltage::toTermName() {
    return "V"+std::to_string(node);
}

bool UnknownVoltage::equals(Unknown *other) {
    if(other->t != Unknown::VOLTAGE){
        return false;
    }

    auto* c = (UnknownVoltage*) other;
    return c->node == node;
}