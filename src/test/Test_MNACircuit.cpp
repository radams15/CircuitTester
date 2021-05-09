//
// Created by rhys on 09/05/2021.
//
#include <gtest/gtest.h>

#include "../main/Analysis/MNACircuit.h"

TEST(MNACircuit, EmptyList){
    auto c = new MNACircuit({});
}

TEST(MNACircuit, GetCurrentTotalNegative){
    auto c = new MNACircuit({});
    c->getCurrentTotal(-1);
}