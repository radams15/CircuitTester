/** @file Test_MNA.cc
 * This tests the modified nodal analysis code
 * @date 09/05/2021
 * @author Rhys Adams
 */

#include <gtest/gtest.h>

#include "../main/Analysis/Equation.h"

#define APROX_EQ(a,b) ASSERT_TRUE((abs(a - b) <= 1E-3))

int testFunc(Unknown* a){
	return 7;
}

/**
 * @brief A or Z NULL
 *
 * Type: Erroneous
 *
 * Data: A = NULL or Z = NULL
 *
 * Expected: controlled error thrown.
 */
TEST(MNAEquation, AZNull1){
    std::vector<Term> empty;
	auto eq =  Equation(3, empty);
	
	//eq.apply(NULL, NULL, *testFunc);
}
