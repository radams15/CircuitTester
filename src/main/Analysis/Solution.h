/**
 * @author Rhys Adams
 * @date 18/04/2021
 */

#ifndef CircuitTester_MNASOLUTION_H
#define CircuitTester_MNASOLUTION_H

#include <map>
#include <vector>
#include <string>

#include "Component.h"

/** @brief Stores the solution of a circuit.
 *
 * This class stores the voltage map, and every component that has
 * a known current so that voltages can be calculated through V=IR.
 *
 */
class Solution {
private:
    /** @brief Check whether double a is equal to double b.
     *
     * This compares two double values by subtracting, getting the
     * absolute value, then comparing the difference to an epsilon,
     * which here is 1x10^-6.
     *
     * @param a The first double
     * @param b The second double
     * @return Whether they are equal or not
     */
    static bool numApproxEquals(double a, double b);

    /** @brief Whether this solution has all the same components
     * as the other solution.
     *
     * @param mnaSolution The other Solution to compare to this.
     * @return Whether the two solutions are equal.
     */
    bool hasAllComponents(Solution mnaSolution);

    /** @brief Determines if component is in this solution.
     *
     * @param component The component to check exists.
     * @return Whether the component is inside the components
     * list.
     */
    bool containsComponent(Component component);

public:
    /** @brief A map of node:voltage.
     *
     */
    std::map<int, double> voltageMap;

    /** @brief A list of components in the solution which
     * have a known current.
     */
    std::vector<Component> components;

    /** @brief Initialiser
     *
     * @param voltageMap A map of node:voltage
     * @param components A list of components in the solution which
     * have a known current.
     */
    Solution(std::map<int, double> voltageMap, std::vector<Component> components);

    /** @brief Whether this has every component and every component is
     * approximately equal to the other in the opposing Solution.
     *
     * @param mnaSolution
     * @return
     */
    bool equals(Solution mnaSolution);

    /** @brief Get the voltage at a specific node.
     *
     * @param nodeIndex The node to check at.
     * @return The voltage at that node.
     */
    double getNodeVoltage(int nodeIndex);

    /** @brief Get the voltage of a specified component.
     *
     * @param component The Component to check at.
     * @return The voltage at that component.
     */
    double getVoltage(Component component);

    /** @brief Gets the current at a resistor through I=V/R
     *
     * @param resistor The resistor to get the current of
     * @return The current at that resistor.
     */
    double getCurrent(Component resistor);
};


#endif //CircuitTester_MNASOLUTION_H
