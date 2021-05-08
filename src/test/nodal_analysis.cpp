//
// Created by rhys on 01/02/2021.
//

#include <gtest/gtest.h>

#include "../main/Analysis/MNAElement.h"
#include "../main/Analysis/MNACircuit.h"
#include "../main/Analysis/MNASolution.h"
#include "../main/Components/Battery.h"
#include "../main/Components/Resistor.h"

TEST(NodalAnalysis, OneResistorCurrent){
    auto* bat = new Battery(0, 1, 4);
    auto* res = new Resistor(1, 0, 4);

    auto cir = new MNACircuit({bat, res});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, 4.0},
    };

    auto dessol = new MNASolution(vmap, {bat});

    auto sol = cir->solve();

    ASSERT_EQ(sol->approxEquals(*dessol), true);
}

TEST(NodalAnalysis, TwoBatterySeries){
    auto bat1 = new Battery(0, 1, -4);
    auto bat2 = new Battery(1, 2, -4);
    auto res = new Resistor(2, 0, 2);

    auto cir = new MNACircuit({bat1, bat2, res});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, -4.0},
            {2, -8.0},
    };

    auto dessol = new MNASolution(vmap, {bat1->withCurrentSolution(-4), bat2->withCurrentSolution(-4)});

    auto sol = cir->solve();

    ASSERT_EQ(sol->approxEquals(*dessol), true);

}