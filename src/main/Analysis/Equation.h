//
// Created by rhys on 18/04/2021.
//

#ifndef CircuitTester_EQUATION_H
#define CircuitTester_EQUATION_H

#include <string>
#include <vector>

#include <Eigen/Core>

#include "MNAElement.h"
#include "Unknown.h"

class Term {
private:

public:
    double coefficient;
    Unknown* variable;

    Term(double coefficient, Unknown* variable);

    std::string str();
};


class Equation {
private:

public:
    double value;
    std::vector<Term*> terms;

    Equation(double value, std::vector<Term*> terms);

    void apply(int row, Eigen::MatrixXd* a, Eigen::MatrixXd* z, std::function<int(Unknown*)> getIndexFunc);

    std::string str();
};


#endif //CircuitTester_EQUATION_H
