/** @file Test_MNA.cc
 * This tests the modified nodal analysis code
 * @date 09/05/2021
 * @author Rhys Adams
 */

#include <gtest/gtest.h>

#include "../main/Analysis/Component.h"
#include "../main/Analysis/Circuit.h"
#include "../main/Analysis/Solution.h"

#define APROX_EQ(a,b) ASSERT_TRUE((abs(a - b) <= 1E-3))

/**
 * @brief One Resistor Current.
 * Tests to see if the current of one 4.0 &Omega; resistor
 * connected to a 10.0V battery is 2.5A.
 */
TEST(NodalAnalysis, OneResistorCurrent){
    auto bat = Component(0, 1, MNA_BATTERY, 10);
    auto res = Component(1, 0, MNA_RESISTOR, 4);

    auto cir = Circuit({bat, res});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 10.0},
    };

    auto dessol = Solution(vmap, {bat});

    auto sol = cir.solve();

    ASSERT_EQ(sol.equals(dessol), true);

    ASSERT_EQ(sol.getCurrent(res), 2.5);
}

/**
 * @brief Two Resistors In Parallel.
 * Tests to see if the currents of two resistors
 * of resistances 10.0 &Omega; and 20.0 &Omega; on a
 * 9.0V battery are 0.9A and 0.45A respectively with a total current
 * of 1.35A to test current splitting.
 */
TEST(NodalAnalysis, TwoResistorsInParallel){
    auto bat = Component(0, 1, MNA_BATTERY, 9);
    auto res1 = Component(2, 0, MNA_RESISTOR, 10);
    auto res2 = Component(2, 0, MNA_RESISTOR, 20);
    auto am1 = Component(1, 2, MNA_RESISTOR, 0.001);

    auto cir = Circuit({bat, res1, res2, am1});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 10.0},
            {2, 10.0},
    };

    auto sol = cir.solve();

    APROX_EQ(sol.getCurrent(*res1), 0.9);
    APROX_EQ(sol.getCurrent(*res2), 0.45);
    APROX_EQ(sol.getCurrent(*am1), 1.35);
}

/**
 * @brief Parallel And Series Resistors.
 *
 * Two resistors in series (5 &Omega; and 10 &Omega;) connected to
 * one 7 &Omega; resistor, connected to a 9V battery.
 *
 * Currents should be 0.58A, 0.29A and 0.87A respectively.
 */
TEST(NodalAnalysis, ParalellAndSeriesResistors){
    auto bat = Component(0, 1, MNA_BATTERY, 9);
    auto res1 = Component(1, 2, MNA_RESISTOR, 5);
    auto res2 = Component(1, 2, MNA_RESISTOR, 10);
    auto res3 = Component(2, 0, MNA_RESISTOR, 7);

    auto cir = Circuit({bat, res1, res2, res3});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 9.0},
            {2, 6.09677},
    };

    auto dessol = Solution(vmap, {bat.withCurrent(0.870968)});

    auto sol = cir.solve();

    ASSERT_EQ(sol.equals(*dessol), true);

    ASSERT_DOUBLE_EQ(round(sol.getCurrent(*res1) * 100) / 100, 0.58);
    ASSERT_DOUBLE_EQ(round(sol.getCurrent(*res2) * 100) / 100, 0.29);
    ASSERT_DOUBLE_EQ(round(sol.getCurrent(*res3) * 100) / 100, 0.87);
}


/**
 * @brief Two Batteries In Series.
 * Tests to see if the voltage of two 9.0V
 * batteries are added together to get a total
 * of 18.0V.
 */
TEST(NodalAnalysis, TwoBatteriesInSeries){
    auto bat1 = Component(0, 1, MNA_BATTERY, 9);
    auto bat2 = Component(1, 2, MNA_BATTERY, 9);
    auto am1 = Component(2, 0, MNA_RESISTOR, 10);

    auto cir = Circuit({bat1, bat2, am1});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 9.0},
            {2, 18.0},
    };

    auto dessol = Solution(vmap, {bat1.withCurrent(1.8), bat2.withCurrent(1.8)});

    auto sol = cir.solve();

    ASSERT_EQ(sol.equals(*dessol), true);

    APROX_EQ(sol.getCurrent(*am1), 1.8);
    APROX_EQ(sol.getVoltage(*am1), 18);
}

/**
 * @brief Two Batteries In Parallel.
 * Tests to see if the voltage of two 4.0V
 * batteries is 4.0V, and only one has a current of
 * 0.4A.
 * Also tests if the values are identical to
 * if there is only 1 4.0V battery.
 */
TEST(NodalAnalysis, TwoBatteriesInParallel){
    auto bat1 = Component(0, 1, MNA_BATTERY, 4);
    auto bat2 = Component(0, 1, MNA_BATTERY, 4);
    auto res = Component(1, 0, MNA_RESISTOR, 10);

    auto cir = Circuit({bat1, bat2, res});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 4.0},
    };

    auto dessol = Solution(vmap, {bat1.withCurrent(0.4), bat2.withCurrent(0.0)});

    auto sol = cir.solve();

    ASSERT_EQ(sol.equals(*dessol), true);
}