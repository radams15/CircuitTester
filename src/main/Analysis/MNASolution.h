/**
 * @author Rhys Adams
 * @date 18/04/2021
 */

#ifndef CircuitTester_MNASOLUTION_H
#define CircuitTester_MNASOLUTION_H

#include <map>
#include <vector>
#include <string>

#include "MNAElement.h"

class MNASolution {
private:
    bool numApproxEquals(double a, double b);

    bool hasAllElements(MNASolution mnaSolution);

    bool containsElement(MNAElement* element);

public:
    std::map<int, double> voltageMap;
    std::vector<MNAElement*> elements;

    MNASolution(std::map<int, double> voltageMap, std::vector<MNAElement*> elements);

    bool equals(MNASolution mnaSolution);

    double getNodeVoltage(int nodeIndex);
    double getVoltage(MNAElement element);

    double getCurrentForResistor(MNAElement resistor);
};


#endif //CircuitTester_MNASOLUTION_H
