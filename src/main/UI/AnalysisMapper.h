/** @brief
 * @author Rhys Adams
 * @date 15/05/2021
 */

#ifndef CIRCUITTESTER_ANALYSISMAPPER_H
#define CIRCUITTESTER_ANALYSISMAPPER_H

#include <vector>
#include <list>

#include "SceneItem.h"

#include "../Analysis/MNASolution.h"
#include "UIComponent.h"

/**
 * \def IS_TYPE(type, ptr)
 * Attempts to find whether \ptr is an instance of \type through a dynamic cast.
 *
 * If they are not the same class, then a dynamic cast returns a nullptr.
*/

#define IS_TYPE(type, ptr) dynamic_cast<type*>(ptr) != nullptr

/** @brief A map of component: list of components.
 *
 * This shows the connections between components as an adjacency list.
 */
typedef std::map<UIComponent*, std::vector<UIComponent*>> Graph;

/** @brief A list of the paths between multiple components on a graph.
 *
 */
typedef std::vector<UIComponent*> Path;

/** @brief Class that maps UI components onto MNA components.
 *
 */
class AnalysisMapper {
private:
    /** @brief List of actual component objects on the canvas*/
    std::vector<UIComponent*> components;

    /** @brief List of arrows on the canvas*/
    std::vector<Arrow*> arrows;

    /** @brief Finds the shortest path between two nodes on a graph using Dijkstra's algorithm.
     *
     * @param graph The graph to traverse.
     * @param start The node to start at.
     * @param end The node to find the shortest path to.
     * @return The shortest path between the start and end components.
     */
    static Path* find_shortest_path(Graph* graph, UIComponent* start, UIComponent* end);

    /** @brief Makes an adjacency list out of the components of the circuit.
     *
     * @return A graph object of connections in the circuit.
     */
    Graph makeGraph();

public:

    /** @brief Initialises AnalysisMapper.
     *
     * @param graphicsItems A list of QGraphicsItems on the circuit canvas.
     */
    explicit AnalysisMapper(std::list<QGraphicsItem*> graphicsItems);

    /** @brief Gets an MNASolution from the circuit canvas.
     *
     * @return MNASolution from the graph.
     */
    MNASolution* getSolution();
};


#endif //CIRCUITTESTER_ANALYSISMAPPER_H
