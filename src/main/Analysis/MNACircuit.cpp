//
// Created by rhys on 18/04/2021.
//

#include "MNACircuit.h"

#include <Eigen/Dense>
#include <iostream>

#define CUR_IN 1
#define CUR_OUT 0

template<typename T>
T MNACircuit::vecPopFront(std::vector<T>& vec){
    std::reverse(vec.begin(),vec.end()); // first becomes last, reverses the vector
    T out = vec.back(); // get last element
    vec.pop_back(); // pop last element off the vector
    std::reverse(vec.begin(),vec.end()); // reverses it again, so the elements are in the same order as before

    return out;
}

MNACircuit::MNACircuit(std::vector<MNAElement *> elements) {
    batteries.clear();
    resistors.clear();
    currentSources.clear();

    this->elements = elements;

    // Split the elements into the 3 types: battery, resistor and current source.
    for(auto e : elements){
        switch(e->type){
            case BATTERY:
                batteries.push_back(e);
                break;
            case RESISTOR:
                resistors.push_back(e);
                break;
            case CURRENT_SRC:
                currentSources.push_back(e);
                break;
        }
    }

    // Populates the nodes list.
    for(auto e : elements){
        nodes.push_back(e->n0);
        nodes.push_back(e->n1);
    }

    // Remove duplicates in nodes by sorting then erasing duplicates.
    std::sort(nodes.begin(), nodes.end());
    nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());

    // Counts the number of nodes.
    nodeCount = nodes.size();
}

int MNACircuit::getNumUnknownCurrents() {
    int freeResistors = 0;
    for(auto r : resistors){
        if(r->value == 0){
            freeResistors++;
        }
    }

    return batteries.size()-freeResistors;
}

int MNACircuit::getNumVars() {
    return nodeCount + getNumUnknownCurrents();
}

double MNACircuit::getCurrentTotal(int nodeIndex) {
    double numCurrentSources = 0.0;

    for(auto c : currentSources){
        if(c->n1 == nodeIndex){
            numCurrentSources -= c->value;
        }

        if(c->n0 == nodeIndex){
            numCurrentSources += c->value;
        }
    }

    return numCurrentSources;
}

std::vector<Term *>* MNACircuit::getCurrents(int node, int side) {
    /*Convert the side to the current direction - if side is 0, current direction
     * is 1, otherwise the current direction is -1*/
    int currentDirection = side == 0 ? 1 : -1;

    if(side != 0 and side != 1) throw std::invalid_argument("Invalid Side!");

    auto* out = new std::vector<Term*>;

    for(auto b : batteries){
        int bside = side == 0? b->n0 : b->n1;

        if(bside == node) {
            out->push_back(new Term(currentDirection, new UnknownCurrent(b)));
        }
    }

    for(auto r : resistors) {
        int rside = side == 0 ? r->n0 : r->n1;

        if (rside == node && r->value == 0) {
            out->push_back(new Term(currentDirection, new UnknownCurrent(r)));
        }else if(rside == node && r->value != 0){
            out->push_back(new Term(-currentDirection / r->value, new UnknownVoltage(r->n1 )));
            out->push_back(new Term(currentDirection / r->value, new UnknownVoltage(r->n0 )));
        }
    }

    return out;
}

std::vector<int>* MNACircuit::getRefNodes() {
    auto* out = new std::vector<int>;

    std::vector<int> toVisit = nodes; // Make a copy of the nodes vector.

    while(! toVisit.empty()){
        int refNodeId = toVisit.at(0);

        out->push_back(refNodeId);

        std::vector<int>* connectedNodes = getConnectedNodes(refNodeId);

        for(auto c : *connectedNodes){
            toVisit.erase(std::remove_if(toVisit.begin(), toVisit.end(), [c](int i){
                return i == c;
            }));
        }
    }

    return out;
}

std::vector<int>* MNACircuit::getConnectedNodes(int node) {
    auto* visited = new std::vector<int>;
    std::vector<int> toVisit = {node};

    while(! toVisit.empty()){
        int nodeToVisit = vecPopFront<int>(toVisit);

        visited->push_back(nodeToVisit);

        for(auto e : elements){
            if(e->contains(nodeToVisit)){
                int oppositeNode = e->opposite(nodeToVisit);
                if(! std::count(visited->begin(), visited->end(), oppositeNode)){
                    toVisit.push_back(oppositeNode);
                }
            }
        }
    }

    visited->erase(unique( visited->begin(), visited->end() ), visited->end());
    return visited;
}

std::vector<Equation *>* MNACircuit::getEquations() {
    auto* equations = new std::vector<Equation *>;

    std::vector<int>* refNodeIds = getRefNodes();

    for(auto r : *refNodeIds){
        equations->push_back(new Equation(0, {
            new Term(1, new UnknownVoltage(r))
        }));
    }

    for(auto n : nodes){
        if(! std::count(refNodeIds->begin(), refNodeIds->end(), n)){
            std::vector<Term *>* incoming = getCurrents(n, CUR_IN);
            std::vector<Term *>* outgoing = getCurrents(n, CUR_OUT);

            auto* conserved = new std::vector<Term *>;
            conserved->insert(conserved->end(), incoming->begin(), incoming->end());
            conserved->insert(conserved->end(), outgoing->begin(), outgoing->end());

            equations->push_back(new Equation(getCurrentTotal(n), *conserved));
        }
    }

    for(auto b : batteries){
        equations->push_back(new Equation(b->value, {
            new Term(-1, new UnknownVoltage(b->n0)),
            new Term(1, new UnknownVoltage(b->n1))
        }));
    }

    for(auto r : resistors){
        if(r->value == 0){
            equations->push_back(new Equation(r->value, {
                    new Term(1, new UnknownVoltage(r->n0)),
                    new Term(-1, new UnknownVoltage(r->n1))
            }));
        }
    }

    return equations;
}

std::vector<UnknownCurrent *> *MNACircuit::getUnknownCurrents() {
    auto* out = new std::vector<UnknownCurrent*>;

    for(auto b : batteries){
        out->push_back(new UnknownCurrent(b));
    }

    for(auto r : resistors){
        if(r->value == 0) {
            out->push_back(new UnknownCurrent(r));
        }
    }

    return out;
}

MNASolution *MNACircuit::solve() {
    std::vector<Equation *>* equations = getEquations();
    std::vector<UnknownCurrent*>* unknownCurrents = getUnknownCurrents();
    auto* unknownVoltages = new std::vector<UnknownVoltage*>;

    // Create an UnknownVoltage for each node as we don't know any voltage for any node.
    for(auto v : nodes){
        unknownVoltages->push_back(new UnknownVoltage(v));
    }

    auto* unknowns = new std::vector<Unknown*>(unknownCurrents->begin(), unknownCurrents->end());

    unknowns->insert(unknowns->end(), unknownVoltages->begin(), unknownVoltages->end());

    //Make two matrices, one which is the width of the number of equations by the number
    // of variables, and the other has a width of the number of equations but is only 1 wide
    auto A = Eigen::MatrixXd(equations->size(), getNumVars()).setZero();
    auto z = Eigen::MatrixXd(equations->size(), 1).setZero();

    for(int i=0 ; i<equations->size() ; i++){
        //Apply the correct numbers of every equation onto the matrices,
        //  passing in a lambda to find the index of each
        //  term of the equation in class attribute unknowns.
        equations->at(i)->apply(i, &A, &z, [this, unknowns](Unknown *u) {
            return getElementIndex<Unknown>(unknowns, u);
        });
    }

    // Solve the matrix equation a = z
    Eigen::MatrixXd x = A.fullPivLu().solve(z); // https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html

    //std::cout << A << "\n\n" << z << "\n\n" << x << "\n\n";

    // A map of which nodes have which voltages on the circuit.
    auto* voltageMap = new std::map<int, double>;

    for(auto v : *unknownVoltages){
        // Get the voltage out from the solved matrix at the index of the element
        // in the vector of unknowns. As the solved matrix stores
        auto voltage = x(getElementIndex<Unknown>(unknowns, v));

        // Add to the voltage map
        voltageMap->insert(std::pair<int, double>(v->node, voltage));
    }

    //
    std::vector<MNAElement*> elems;

    for(auto c : *unknownCurrents){
        // Set the new current for each element, as the matrix column 0 has the solved currents.
        c->element->currentSolution = x(getElementIndex<Unknown>(unknowns, c), 0);
        elems.push_back(c->element);

    }

    return new MNASolution(*voltageMap, elems);
}

template <typename T>
int MNACircuit::getElementIndex(std::vector<T*>* array, T* element) {
    for(int i=0 ; i<array->size() ; i++){
        if(array->at(i)->equals(element)){
            return i;
        }
    }

    return -1;
}
