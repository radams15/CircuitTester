/** @file Test_MNA.cc
 * This tests the modified nodal analysis code
 * @date 09/05/2021
 * @author Rhys Adams
 */

#include <gtest/gtest.h>

#include "../main/Analysis/Component.h"

#define APROX_EQ(a,b) ASSERT_TRUE((abs(a - b) <= 1E-3))

/**
 * @brief Component contains valid node
 *
 * Type: Normal
 *
 * Data: contains(2)
 *
 * Expected: true
 */
TEST(MNAComponent, ContainsNormal){
    auto bat = Component(2, 3, MNA_RESISTOR, 10);
    
    ASSERT_EQ(bat.contains(2), true);
}

/**
 * @brief Component does not contain valid node
 *
 * Type: Normal
 *
 * Data: contains(100)
 *
 * Expected: false
 */
TEST(MNAComponent, ContainsNormal2){
    auto bat = Component(2, 3, MNA_RESISTOR, 10);
    
    ASSERT_EQ(bat.contains(100), false);
}

/**
 * @brief Component invalid opposite node
 *
 * Type: Erroneous
 *
 * Data: opposite(100)
 *
 * Expected: controlled error thrown
 */
TEST(MNAComponent, OpposideInvalid){
    auto bat = Component(2, 3, MNA_RESISTOR, 10);
    
    //ASSERT_EQ(bat.opposite(100), false);
}
