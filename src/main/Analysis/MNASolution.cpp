//
// Created by rhys on 18/04/2021.
//

#include <iostream>
#include "MNASolution.h"

MNASolution::MNASolution(std::map<int, double> nodeVoltages, std::vector<MNAElement *> elements) {
    this->nodeVoltages = nodeVoltages;
    this->elements = elements;
}

bool MNASolution::approxEquals(MNASolution otherMnaSolution) {
    std::vector<int> keys;
    std::vector<int> otherKeys;

    for(auto n : nodeVoltages){
        keys.push_back(n.first);
    }

    for(auto n : otherMnaSolution.nodeVoltages){
        otherKeys.push_back(n.first);
    }

    for(auto key : keys){
        if(!numApproxEquals(getNodeVoltage(key), otherMnaSolution.getNodeVoltage(key))){
            return false;
        }
    }

    if((!hasAllCurrents(otherMnaSolution))){
        return false;
    }

    if(!otherMnaSolution.hasAllCurrents(*this)) {
        return false;
    }

    return true;
}

bool MNASolution::numApproxEquals(double a, double b) {
    return std::abs(a-b) < 1e-6;
}

double MNASolution::getNodeVoltage(int nodeIndex) {
    return nodeVoltages.at(nodeIndex);
}

double MNASolution::getCurrentForResistor(MNAElement resistor) {
    return -getVoltage(resistor) / resistor.value;
}

double MNASolution::getVoltage(MNAElement element) {
    return nodeVoltages.at(element.n1) - nodeVoltages.at(element.n0);
}

bool MNASolution::hasAllCurrents(MNASolution mnaSolution) {
    for(auto e : mnaSolution.elements){
        if(!hasMatchingElement(e)){
            return false;
        }
    }

    return true;
}

bool MNASolution::hasMatchingElement(MNAElement *element) {
    for(auto e : elements){
        if(e->n0 == element->n0 && e->n1 == element->n1 && numApproxEquals(e->currentSolution, element->currentSolution)){
            return true;
        }
    }

    return false;
}
