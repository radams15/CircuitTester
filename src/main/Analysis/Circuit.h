/** @brief
 * @author Rhys Adams
 * @date 18/04/2021
 */

#ifndef CircuitTester_MNACIRCUIT_H
#define CircuitTester_MNACIRCUIT_H

#include <vector>
#include <map>
#include <string>

#include "Component.h"
#include "Equation.h"
#include "Solution.h"

/** @brief Main class that holds a circuit for
 * modified nodal analysis (MNA).
 *
 */
class Circuit {
private:
    /** @brief Holds all the MNAComponents that are batteries */
    std::vector<Component> batteries;

    /** @brief Holds all the MNAComponents that are resistors */
    std::vector<Component> resistors;

    /** @brief Holds all the MNAComponents combined */
    std::vector<Component> components;

    /** @brief The total number of nodes */
    int nodeCount;

    /** @brief All the nodes in the circuit*/
    std::vector<int> nodes;

    /** @brief Pops and returns the first component of the vector vec.
     *
     * This was written as the STL only has a pop_back method, not
     * a pop_front method.
     *
     * @tparam T The type that is in the vector.
     * @param vec A vector to remove the first component of
     * @return The first component that was removed.
     */
    template<typename T>
    T vecPopFront(std::vector<T>& vec);

    /** @brief Gets the number of unknown currents.
     *
     * Current sources are not calculated as they are known currents.
     *
     * @return The number of unknown currents.
     */
    int getNumUnknownCurrents();

    /** @brief Used to find out the number of both current and voltage variables.
     *
     * @return The total number of variables in the circuit.
     */
    int getNumVars();

    /** @brief Selects the components that have the reference voltage.
     *
     * This is the voltage that other components are compared to.
     * There is 1 reference node per circuit.
     *
     * @return A list of nodes that are references.
     */
    std::vector<int> getRefNodes();

    /** @brief Returns a list of equations that must be solved
     * to solve the overall circuit.
     *
     * @return A list of solvable Equation objects.
     */
    std::vector<Equation> getEquations();

    /** @brief Returns all unknown currents in the circuit
     *
     * @return A list of all the UnknownCurrent objects.
     */
    std::vector<UnknownCurrent*> getUnknownCurrents();

public:
    /** @brief Initialises Circuit object.
     *
     * This initialises the Circuit, as well as categorising
     * the components into the batteries, resistors and currentSources
     * vectors. Also generates nodeSet, nodeCount and nodes variables.
     *
     * @param components The list of all circuit components.
    */
    explicit Circuit(std::vector<Component> components);

    /** @brief Returns the index of the component in the array.
 *
 * If it does not exist, returns -1.
 *
 * @tparam T The type of the vector, and of the component.
 * @param array The vector of T's.
 * @param component The component to search for.
 * @return The component index in the vector, or -1 if the component is not found.
 */
    template <typename T>
    int getComponentIndex(std::vector<T*> array, T* component);

    /** @brief Solves the circuit using linear algebra and matrices.
     *
     * @return An Solution class for this circuit.
     */
    Solution solve();

    /** Finds the nodes that are connected to the passed node.
     *
     * @param node The node to search around.
     * @return
     */
    std::vector<int> getConnectedNodes(int node);

    /** @brief Gets the terms that enter and leave a node.
     *
     * Incoming is negative, outgoing is positive
     *
     * @param node The node to search around.
     * @param side The side we are referring to - 1 for incoming, 0 for outgoing.
     * @return
     */
    std::vector<Term> getCurrents(int node, int side);
};

#endif //CircuitTester_MNACIRCUIT_H
