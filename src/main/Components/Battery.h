//
// Created by rhys on 08/05/2021.
//

#ifndef CIRCUITTESTER_BATTERY_H
#define CIRCUITTESTER_BATTERY_H


#include "../Analysis/MNAElement.h"

class Battery : public MNAElement{
public:
    Battery(int n0, int n1, double value, double currentSolution=NAN);
};


#endif //CIRCUITTESTER_BATTERY_H
