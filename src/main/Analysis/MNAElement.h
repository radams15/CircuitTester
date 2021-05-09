//
// Created by rhys on 18/04/2021.
//

#ifndef CircuitTester_MNAELEMENT_H
#define CircuitTester_MNAELEMENT_H

#include <cmath>

enum ElementType{
    BATTERY,
    RESISTOR,
    CURRENT_SRC
};

class MNAElement {
private:

public:
    int n0;
    int n1;
    double value;
    double currentSolution;

    ElementType type;

    MNAElement(int n0, int n1, ElementType type, double value, double currentSolution=NAN);

    MNAElement* withCurrentSolution(double newCurrentSolution);

    bool contains(int n);

    int opposite(int n);

    bool equals(MNAElement other);
};


#endif //CircuitTester_MNAELEMENT_H
