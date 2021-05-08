//
// Created by rhys on 08/05/2021.
//

#ifndef CIRCUITTESTER_CURRENTSOURCE_H
#define CIRCUITTESTER_CURRENTSOURCE_H

#include "../Analysis/MNAElement.h"

class CurrentSource : public MNAElement{
public:
    CurrentSource(int n0, int n1, double value, double currentSolution=NAN);
};

#endif //CIRCUITTESTER_CURRENTSOURCE_H
