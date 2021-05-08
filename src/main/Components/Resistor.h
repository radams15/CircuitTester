//
// Created by rhys on 08/05/2021.
//

#ifndef CIRCUITTESTER_RESISTOR_H
#define CIRCUITTESTER_RESISTOR_H

#include "../Analysis/MNAElement.h"

class Resistor : public MNAElement{
public:
    Resistor(int n0, int n1, double value, double currentSolution=NAN);
};

#endif //CIRCUITTESTER_RESISTOR_H
