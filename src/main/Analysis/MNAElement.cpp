//
// Created by rhys on 18/04/2021.
//

#include <iostream>
#include "MNAElement.h"

MNAElement::MNAElement(int n0, int n1, ElementType type, double value, double currentSolution) {
    this->n0 = n0;
    this->n1 = n1;

    this->type = type;

    this->value = value;
    this->currentSolution = currentSolution;
}

MNAElement* MNAElement::withCurrentSolution(double newCurrentSolution) {
    return new MNAElement(n0, n1, type, value, newCurrentSolution);
}

bool MNAElement::contains(int n) {
    return n == n0 || n == n1;
}

int MNAElement::opposite(int n) {
    return n == n0? n1 : n0;
}

bool MNAElement::equals(MNAElement other) {
    if(other.n0 == n0 && other.n1 == n1){
        return true;
    }

    return false;
}
