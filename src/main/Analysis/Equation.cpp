//
// Created by rhys on 18/04/2021.
//

#include <iostream>
#include "Equation.h"

Term::Term(double coefficient, Unknown *variable) {
    this->coefficient = coefficient;
    this->variable = variable;
}

std::string Term::str() {
    std::string prefix = coefficient == 1? "" : coefficient == -1? "-" : std::to_string(coefficient);

    return prefix + variable->toTermName();
}

Equation::Equation(double value, std::vector<Term*> terms) {
    this->value = value;
    this->terms = terms;
}

void Equation::apply(int row, Eigen::MatrixXd* a, Eigen::MatrixXd* z, std::function<int(Unknown*)> getIndexFunc) {
    (*z)(row, 0) = value;

    for(auto t : terms){
        int index = getIndexFunc(t->variable);
        (*a)(row, index) = t->coefficient + (*a)(row, index);
    }
}

std::string Equation::str() {
    std::stringstream termList;

    for(auto t : terms){
        termList << t->str() << " +";
    }

    return "{"+termList.str()+"} = " + std::to_string(value);
}

Unknown::Unknown(Type t) {
    this->t = t;
}
