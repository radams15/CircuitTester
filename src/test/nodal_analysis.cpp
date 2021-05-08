/** @file nodal_analysis.cpp
 * This tests the modified nodal analysis code
 */
//
// Created by rhys on 01/02/2021.
//

#include <gtest/gtest.h>

#include "../main/Analysis/MNAElement.h"
#include "../main/Analysis/MNACircuit.h"
#include "../main/Analysis/MNASolution.h"
#include "../main/Components/Battery.h"
#include "../main/Components/Resistor.h"

/**
 * @brief One Resistor Current.
 * Tests to see if the current of one 4.0 &Omega; resistor
 * connected to a 10.0V battery is 2.5A.
 */
TEST(NodalAnalysis, OneResistorCurrent){
    auto bat = new Battery(0, 1, 10);
    auto res = new Resistor(1, 0, 4);

    auto cir = new MNACircuit({bat, res});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 10.0},
    };

    auto dessol = new MNASolution(vmap, {bat});

    auto sol = cir->solve();

    ASSERT_EQ(sol->approxEquals(*dessol), true);

    ASSERT_EQ(sol->getCurrentForResistor(*res), 2.5);
}


/**
 * @brief Two Resistors In Series.
 * Tests to see if the currents of two resistors
 * of resistances 10.0 &Omega; and 20.0 &Omega; on a
 * 10V battery are 1.0A and 0.5A respectively to test
 * current splitting.
 */
TEST(NodalAnalysis, TwoResistorsInSeries){
    auto bat = new Battery(0, 1, 10);
    auto res1 = new Resistor(1, 0, 10);
    auto res2 = new Resistor(1, 0, 20);

    auto cir = new MNACircuit({bat, res1, res2});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 10.0},
            {2, 10.0},
    };

    auto dessol = new MNASolution(vmap, {bat->withCurrentSolution(1.5)});

    auto sol = cir->solve();

    ASSERT_EQ(sol->approxEquals(*dessol), true);

    ASSERT_EQ(sol->getCurrentForResistor(*res1), 1.0);
    ASSERT_EQ(sol->getCurrentForResistor(*res2), 0.5);
}

/**
 * @brief Two Batteries In Series.
 * Tests to see if the voltage of two 4.0V
 * batteries are added together to get a total
 * of 8.0V.
 */
TEST(NodalAnalysis, TwoBatteriesInSeries){
    auto bat1 = new Battery(0, 1, 4);
    auto bat2 = new Battery(1, 2, 4);
    auto res = new Resistor(2, 0, 2);

    auto cir = new MNACircuit({bat1, bat2, res});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 4.0},
            {2, 8.0},
    };

    auto dessol = new MNASolution(vmap, {bat1->withCurrentSolution(4), bat2->withCurrentSolution(4)});

    auto sol = cir->solve();

    ASSERT_EQ(sol->approxEquals(*dessol), true);
}

/**
 * @brief Two Batteries In Parallel.
 * Tests to see if the voltage of two 4.0V
 * batteries is 4.0V, and have a current of
 * 0.2A.
 * Also tests if the values are identical to
 * if there is only 1 4.0V battery.
 */
TEST(NodalAnalysis, TwoBatteriesInParallel){
    auto bat1 = new Battery(0, 1, 4);
    auto bat2 = new Battery(0, 1, 4);
    auto res = new Resistor(1, 0, 10);

    auto cir = new MNACircuit({bat1, bat2, res});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 4.0},
    };

    auto dessol = new MNASolution(vmap, {bat1->withCurrentSolution(0.4), bat2->withCurrentSolution(0.0)});

    auto sol = cir->solve();

    for(auto v : sol->elements){
        std::cout << v->n0 << "=>" << v->n1 << " @ " << v->currentSolution << std::endl;
    }

    for(auto v : sol->nodeVoltages){
        std::cout << v.first << ":" << v.second << std::endl;
    }

    ASSERT_EQ(sol->approxEquals(*dessol), true);
}