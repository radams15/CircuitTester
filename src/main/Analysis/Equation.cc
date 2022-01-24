//
// Created by rhys on 18/04/2021.
//

#include <iostream>
#include "Equation.h"

#include "Circuit.h"

#define foreach(a, b) for(unsigned int i=0 ; i<b.size() ; i++){a = b[i];

Equation::Equation(double value, std::vector<Term> terms) {
    this->value = value;
    this->terms = terms;
}

void Equation::apply(int row, Eigen::MatrixXd* A, Eigen::MatrixXd* z, std::vector<Unknown*> unknowns, Circuit* c) {
    // Sets the value of the equation to the correct section on the z matrix.
    (*z)(row, 0) = value;

    foreach(Term t, terms)
        // Find the index of the unknown variable of this term.
        int index = c->getComponentIndex<Unknown>(unknowns, t.variable);

        // Add the known coefficient of this term into the matrix A,
        // adding onto the existing value there.
        (*A)(row, index) += t.coefficient;
    }
}
