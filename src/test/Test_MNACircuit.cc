//
// Created by rhys on 09/05/2021.
//
#include <gtest/gtest.h>

#include "../main/Analysis/MNACircuit.h"

/** @brief Initialiser empty list
 *
 * Type: boundary
 *
 * Data: [ ]
 *
 * Expected: No error, as there is just no circuit.
 */
TEST(MNACircuit, EmptyList){
    auto c = new MNACircuit({});
}

/** @brief getConnectedNodes invalid node
 *
 * Type: invalid
 *
 * Data: 100
 *
 * Expected: Returns empty list
 */
TEST(MNACircuit, GetConnectedNodesInvalidNode){
    auto c = new MNACircuit({});
    ASSERT_EQ(*(c->getConnectedNodes(100)), std::vector<int>(1, 100));
}
