/** @file Test_MNACircuit.cc
 * This tests the modified nodal analysis circuit class.
 * @date 09/05/2021
 * @author Rhys Adams
 */
#include <gtest/gtest.h>

#define private public

#include "../main/Analysis/MNACircuit.h"

/** \def ASSERT_VEC_LEN
 *
 * @brief Assets the length of a vector.
 *
 */
#define ASSERT_VEC_LEN(vec, len) ASSERT_EQ((vec).size(), len)

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

    ASSERT_VEC_LEN(c->batteries, 0); // Are there 0 batteries?
    ASSERT_VEC_LEN(c->resistors, 0); // Are there 0 resistor?
    ASSERT_VEC_LEN(c->elements, 0); // Are there 0 elements overall?
}


/** @brief Correct categorisation.
 *
 * Type: normal
 *
 * Data: [ battery, resistor, battery ]
 *
 * Expected: The MNACircuit correctly places the batteries into a list,
 * and the resistor into a separate list.
 */
TEST(MNACircuit, CorrectCategorisation){
    auto* b1 = new MNAComponent(0, 1, MNA_BATTERY, 2);
    auto* b2 = new MNAComponent(1, 2, MNA_BATTERY, 4);
    auto* r1 = new MNAComponent(2, 0, MNA_RESISTOR, 1);

    auto c = new MNACircuit({b1, r1, b2});

    ASSERT_VEC_LEN(c->batteries, 2); // Are there 2 batteries?
    ASSERT_VEC_LEN(c->resistors, 1); // Is there 1 resistor?
    ASSERT_VEC_LEN(c->elements, 3); // Are there 3 elements overall?
}

/** @brief Correct nodes.
 *
 * Type: normal
 *
 * Data: [ battery(0->1), resistor(1->2), battery(2->0) ]
 *
 * Expected: The MNACircuit correctly identifies the nodes used: 0, 1, 2.
 */
TEST(MNACircuit, CorrectNodes){
    auto* b1 = new MNAComponent(0, 1, MNA_BATTERY, 2);
    auto* b2 = new MNAComponent(1, 2, MNA_BATTERY, 4);
    auto* r1 = new MNAComponent(2, 0, MNA_RESISTOR, 1);

    auto c = new MNACircuit({b1, r1, b2});

    std::vector<int> expected = {0, 1, 2}; // These are the nodes that are used.

    ASSERT_EQ(c->nodes, expected); // Are the nodes correct?
    ASSERT_EQ(c->nodeCount, 3); // Are there 3 nodes?
}

/** @brief Correct number of unknown currents.
 *
 * Type: normal
 *
 * Data: [ battery(0->1), resistor(1->2), battery(2->0) ]
 *
 * Expected: The MNACircuit correctly identifies the number of unknown currents: 2.
 */
TEST(MNACircuit, CorrectNumUnknownCurrents){
    auto* b1 = new MNAComponent(0, 1, MNA_BATTERY, 2);
    auto* b2 = new MNAComponent(1, 2, MNA_BATTERY, 4);
    auto* r1 = new MNAComponent(2, 0, MNA_RESISTOR, 1);

    auto c = new MNACircuit({b1, r1, b2});

    ASSERT_EQ(c->getNumUnknownCurrents(), 2); // Are the number of unknown currents correct?
}

/** @brief Correct number of variables.
 *
 * Type: normal
 *
 * Data: [ battery(0->1), resistor(1->2), battery(2->0) ]
 *
 * Expected: The MNACircuit correctly identifies the number of variables: 3.
 */
TEST(MNACircuit, CorrectNumVars){
    auto* b1 = new MNAComponent(0, 1, MNA_BATTERY, 2);
    auto* b2 = new MNAComponent(1, 2, MNA_BATTERY, 4);
    auto* r1 = new MNAComponent(2, 0, MNA_RESISTOR, 1);

    auto c = new MNACircuit({b1, r1, b2});

    ASSERT_EQ(c->getNumVars(), 5); // Are the number of variables correct?
}

/** @brief Correct reference nodes.
 *
 * Type: normal
 *
 * Data: [ battery(0->1), resistor(1->2), battery(2->0) ]
 *
 * Expected: Reference node of 0 returned.
 */
TEST(MNACircuit, CorrectRefNodes){
    auto* b1 = new MNAComponent(0, 1, MNA_BATTERY, 2);
    auto* b2 = new MNAComponent(1, 2, MNA_BATTERY, 4);
    auto* r1 = new MNAComponent(2, 0, MNA_RESISTOR, 1);

    auto c = new MNACircuit({b1, r1, b2});

    ASSERT_EQ(*(c->getRefNodes()), std::vector<int>(1, 0)); // Are the reference nodes correct?
}

/** @brief Correct reference nodes when there are 2 separate circuits.
 *
 * Type: boundary
 *
 * Data: [ battery(0->1), resistor(1->2), battery(3->4), battery(4->3) ]
 *
 * Expected: Reference nodes of 0 and 3 returned.
 */
TEST(MNACircuit, CorrectRefNodes2){
    auto* b1 = new MNAComponent(0, 1, MNA_BATTERY, 2);
    auto* r1 = new MNAComponent(2, 0, MNA_RESISTOR, 1);

    auto* b2 = new MNAComponent(3, 4,  MNA_BATTERY, 4);
    auto* b3 = new MNAComponent(4, 3, MNA_BATTERY, 4);

    auto c = new MNACircuit({b1, r1, b2, b3});

    std::vector<int> expected = {0, 3};

    ASSERT_EQ(*(c->getRefNodes()), expected); // Are the reference nodes correct?
}

/** @brief getConnectedNodes invalid node
 *
 * Type: invalid
 *
 * Data: 100
 *
 * Expected: Returns list of only itself.
 */
TEST(MNACircuit, GetConnectedNodesInvalidNode){
    auto c = new MNACircuit({});
    ASSERT_EQ(*(c->getConnectedNodes(100)), std::vector<int>(1, 100));
}

/** @brief getCurrentTotal negative
 *
 * Type: invalid
 *
 * Data: node=-1, side=0
 *
 * Expected: Returns empty vector.
 */
TEST(MNACircuit, GetCurrentsNegative){
    auto c = new MNACircuit({});
    ASSERT_VEC_LEN(*(c->getCurrents(-1, 0)), 0);
}

/** @brief getCurrentTotal large number
 *
 * Type: invalid
 *
 * Data: node=100, side=0
 *
 * Expected: Returns empty vector.
 */
TEST(MNACircuit, GetCurrentsLargeNum){
    auto c = new MNACircuit({});
    ASSERT_VEC_LEN(*(c->getCurrents(100, 0)), 0);
}

/** @brief getCurrentTotal invalid sign
 *
 * Type: invalid
 *
 * Data: node=1, side=-2
 *
 * Expected: Controlled error thrown.
 */
TEST(MNACircuit, GetCurrentsInvalidSign){
    auto c = new MNACircuit({new MNAComponent(0, 1, MNA_BATTERY, 2)});
    EXPECT_THROW(c->getCurrents(1, -2), std::exception);
}
