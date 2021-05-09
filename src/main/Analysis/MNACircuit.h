//
// Created by rhys on 18/04/2021.
//

#ifndef CircuitTester_MNACIRCUIT_H
#define CircuitTester_MNACIRCUIT_H

#include <vector>
#include <map>
#include <string>

#include "MNAElement.h"
#include "Equation.h"
#include "MNASolution.h"

class MNACircuit {
private:

public:
    std::vector<MNAElement*> batteries;
    std::vector<MNAElement*> resistors;
    std::vector<MNAElement*> currentSources;

    std::vector<MNAElement*> elements;

    std::map<int, int> nodeSet;
    int nodeCount;
    std::vector<int> nodes;

    explicit MNACircuit(std::vector<MNAElement *> elements);

    int getNumUnknownCurrents();

    int getNumVars();

    double getCurrentTotal(int nodeIndex);

    std::vector<Term*>* getCurrents(int node, int side, int sign);

    std::vector<int>* getRefNodes();

    std::vector<int>* getConnectedNodes(int node);

    std::vector<Equation*>* getEquations();

    std::vector<UnknownCurrent*>* getUnknownCurrents();

    int getElementIndex(std::vector<Unknown*>* array, Unknown* element);

    MNASolution* solve();
};


#endif //CircuitTester_MNACIRCUIT_H
