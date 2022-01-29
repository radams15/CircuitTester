//
// Created by rhys on 18/04/2021.
//

#include <iostream>
#include "Component.h"

Component::Component(int n0, int n1, ComponentType type, double value) {
    // Sets the class attributes from the passed initialiser values.
    this->n0 = n0;
    this->n1 = n1;

    this->type = type;

    if(value <= 0){
        std::cerr << "Cannot have a zero-valued component (" << value << ")" << std::endl;
        throw std::exception();
    }

    this->value = value;
}

Component::Component() {
	std::cerr << "Initialised component without data, probably from indexing a QMap with an invalid key" << std::endl;
}

bool Component::contains(int n) {
    // n is contained if n0 or n1 are equal to n.
    return n == n0 || n == n1;
}

int Component::opposite(int n) {
    // The opposite of n1 is n0, and vice-versa.
    return n == n0? n1 : n0;
}

bool Component::equals(Component other) {
    // If n0, n1 and type from other are the same as this, they are equal.
    return other.n0 == n0 && other.n1 == n1 && other.type == type;
}
