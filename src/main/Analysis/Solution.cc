//
// Created by rhys on 18/04/2021.
//

#include <iostream>
#include "Solution.h"

#define APPROX_EPSILON 1e-6

#define foreach(a, b) for(unsigned int i=0 ; i<b.size() ; i++){a = b[i];

Solution::Solution(std::map<int, double> voltageMap, std::vector<Component> components) {
    // Setup class variables.
    this->voltageMap = voltageMap;
    this->components = components;
}

bool Solution::equals(Solution mnaSolution) {
    std::vector<int> nodes; // The nodes of this circuit.
    std::vector<int> otherNodes; // The nodes of the circuit to compare to.

    for(std::map<int, double>::iterator n = voltageMap.begin(); n != voltageMap.end(); ++n){
        nodes.push_back(n->first);
    }

    for(std::map<int, double>::iterator n = mnaSolution.voltageMap.begin(); n != mnaSolution.voltageMap.end(); ++n){
        otherNodes.push_back(n->first);
    }

    foreach(int key, nodes)
        // Check if every key in this equals the same key in the solution.
        if(!numApproxEquals(getNodeVoltage(key), mnaSolution.getNodeVoltage(key))){
            return false;
        }
    }

    // Check if all the currents in this are in solution.
    if((!hasAllComponents(mnaSolution))){
        return false;
    }

    // Check if all the currents in solution are in this.
    if(!mnaSolution.hasAllComponents(*this)) {
        return false;
    }

    // All checks passed, must be equal.
    return true;
}

double Solution::getNodeVoltage(int nodeIndex) {
    // Gets the voltage from the voltageMap.
    return voltageMap[nodeIndex];
}

double Solution::getCurrent(Component resistor) {
    // Returns the current by doing V=IR, which is equal to I=V/R.
    double v = getVoltage(resistor);
    double r = resistor.value;
    double i = v / r;

    return  i;
}

double Solution::getVoltage(Component component) {
    // Gets the difference between the voltages the start and end nodes
    // as voltage is the potential difference between two components.
    return std::abs(voltageMap[component.n1] - voltageMap[component.n0]);
}

bool Solution::hasAllComponents(Solution mnaSolution) {
    // Return whether containsComponent returns true for every
    // component in mnaSolution.

    foreach(Component c, mnaSolution.components)
        if(!containsComponent(c)) {
            return false;
        }
    }

    return true;
}

bool Solution::containsComponent(Component component) {
    // Returns whether any of the components in this are equal
    // to the passed component.

    foreach(Component c, components)
        if(c.equals(component)) {
            return true;
        }
    }

    return false;
}

bool Solution::numApproxEquals(double a, double b) {
    // Finds the difference between a and b then compares the difference
    // to an epsilon.
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * APPROX_EPSILON);
}
