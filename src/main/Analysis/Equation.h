//
// Created by rhys on 18/04/2021.
//

#ifndef CircuitTester_EQUATION_H
#define CircuitTester_EQUATION_H

#include <string>
#include <vector>

#include <Eigen/Core>

#include "MNAElement.h"

class Unknown {
private:

public:
    enum Type{
        CURRENT,
        VOLTAGE
    };

    Type t;

    virtual bool equals(Unknown* other) = 0;
    virtual std::string toTermName() = 0;

    Unknown(Type t);
};

class UnknownCurrent : public Unknown {
private:

public:
    MNAElement* element;

    explicit UnknownCurrent(MNAElement* element);

    std::string toTermName() override;
    bool equals(Unknown* other) override;
};

class UnknownVoltage : public Unknown {
private:

public:
    int node;

    explicit UnknownVoltage(int node);

    std::string toTermName() override;
    bool equals(Unknown* other) override;
};


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

    void stamp(int row, Eigen::MatrixXd* a, Eigen::MatrixXd* z, std::function<int(Unknown*)> getIndexFunc);

    std::string str();
};


#endif //CircuitTester_EQUATION_H
