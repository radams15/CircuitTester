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

    bool containsNode(int n);

    int getOppositeNode(int n);

    bool equals (const MNAElement& other) const;
};


#endif //CircuitTester_MNAELEMENT_H
