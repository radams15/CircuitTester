//
// Created by rhys on 18/04/2021.
//

#include <iostream>
#include "MNASolution.h"

MNASolution::MNASolution(std::map<int, double> voltageMap, std::vector<MNAElement *> elements) {
    // Setup class variables.
    this->voltageMap = voltageMap;
    this->elements = elements;
}

bool MNASolution::equals(MNASolution mnaSolution) {
    std::vector<int> nodes; // The nodes of this circuit.
    std::vector<int> otherNodes; // The nodes of the circuit to compare to.

    for(auto n : voltageMap){
        nodes.push_back(n.first);
    }

    for(auto n : mnaSolution.voltageMap){
        otherNodes.push_back(n.first);
    }

    for(auto key : nodes){
        if(!numApproxEquals(getNodeVoltage(key), mnaSolution.getNodeVoltage(key))){
            return false;
        }
    }

    if((!hasAllElements(mnaSolution))){
        return false;
    }

    if(!mnaSolution.hasAllElements(*this)) {
        return false;
    }

    return true;
}

bool MNASolution::numApproxEquals(double a, double b) {
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * 1e-6);
}

double MNASolution::getNodeVoltage(int nodeIndex) {
    return voltageMap.at(nodeIndex);
}

double MNASolution::getCurrentForResistor(MNAElement resistor) {
    return -getVoltage(resistor) / resistor.value;
}

double MNASolution::getVoltage(MNAElement element) {
    return voltageMap.at(element.n1) - voltageMap.at(element.n0);
}

bool MNASolution::hasAllElements(MNASolution mnaSolution) {
    for(auto e : mnaSolution.elements){
        if(!containsElement(e)){
            return false;
        }
    }

    return true;
}

bool MNASolution::containsElement(MNAElement *element) {
    for(auto e : elements){
        if(e->n0 == element->n0 && e->n1 == element->n1 && numApproxEquals(e->currentSolution, element->currentSolution)){
            return true;
        }
    }

    return false;
}
