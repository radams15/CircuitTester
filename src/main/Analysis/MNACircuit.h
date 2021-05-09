/** @brief Circuit class for the modified nodal analysis
 * @author Rhys Adams
 * @date 18/04/2021
 */

#ifndef CircuitTester_MNACIRCUIT_H
#define CircuitTester_MNACIRCUIT_H

#include <vector>
#include <map>
#include <string>

#include "MNAElement.h"
#include "Equation.h"
#include "MNASolution.h"

class MNACircuit {
private:
    /** @brief Holds all the MNAElements that are batteries */
    std::vector<MNAElement*> batteries;

    /** @brief Holds all the MNAElements that are resistors */
    std::vector<MNAElement*> resistors;

    /** @brief Holds all the MNAElements that are current sources */
    std::vector<MNAElement*> currentSources;

    /** @brief Holds all the MNAElements combined */
    std::vector<MNAElement*> elements;

    /** @brief A map of node names against node values */
    std::map<int, int> nodeSet;

    /** @brief The total number of nodes */
    int nodeCount;

    /** @brief All the nodes in the circuit*/
    std::vector<int> nodes;

    int getNumUnknownCurrents();

    int getNumVars();

    std::vector<int>* getRefNodes();

    std::vector<Equation*>* getEquations();

    std::vector<UnknownCurrent*>* getUnknownCurrents();

    int getElementIndex(std::vector<Unknown*>* array, Unknown* element);

public:
    /** @brief Initialises MNACircuit object.
     * This initialises the MNACircuit, as well as categorising
     * the elements into the batteries, resistors and currentSources
     * vectors. Also generates nodeSet, nodeCount and nodes variables.
     *
     * @param elements The list of all circuit elements.
    */
    explicit MNACircuit(std::vector<MNAElement *> elements);

    /** @brief Solves the circuit using linear algebra and matrices.
     *
     * @return An MNASolution class for this circuit.
     */
    MNASolution* solve();

    double getCurrentTotal(int nodeIndex);

    std::vector<int>* getConnectedNodes(int node);

    std::vector<Term*>* getCurrents(int node, int side, int sign);
};

#endif //CircuitTester_MNACIRCUIT_H
