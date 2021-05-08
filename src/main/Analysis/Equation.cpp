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

UnknownCurrent::UnknownCurrent(MNAElement *element) : Unknown(CURRENT) {
    this->element = element;
}

std::string UnknownCurrent::toTermName() {
    return "I"+std::to_string(element->n0)+"_"+std::to_string(element->n1);
}

bool UnknownCurrent::equals(Unknown* other) {
    if(other->t != Unknown::CURRENT){
        return false;
    }

    auto* c = (UnknownCurrent*) other;
    return c->element == element;
}

UnknownVoltage::UnknownVoltage(int node)  : Unknown(VOLTAGE) {
    this->node = node;
}

std::string UnknownVoltage::toTermName() {
    return "V"+std::to_string(node);
}

bool UnknownVoltage::equals(Unknown *other) {
    if(other->t != Unknown::VOLTAGE){
        return false;
    }

    auto* c = (UnknownVoltage*) other;
    return c->node == node;
}

Equation::Equation(double value, std::vector<Term*> terms) {
    this->value = value;
    this->terms = terms;
}

void Equation::stamp(int row, Eigen::MatrixXd* a, Eigen::MatrixXd* z, std::function<int(Unknown*)> getIndexFunc) {
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
