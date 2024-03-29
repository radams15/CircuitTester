//
// Created by rhys on 15/05/2021.
//

#include <queue>
#include "AnalysisMapper.h"

#include "UIComponent.h"
#include "Components/Battery.h"
#include "Components/Resistor.h"
#include "Components/Wire.h"
#include "Components/Switch.h"
#include "Line.h"

#include <iostream>

#include "../Analysis/Circuit.h"

AnalysisMapper::AnalysisMapper(std::list<QGraphicsItem*> graphicsItems) {

    // Sorts the QGraphicsItems into UIComponents and Arrows.
    foreach(QGraphicsItem* i, graphicsItems){
        if(IS_TYPE(UIComponent, i)) {
            components.push_back((UIComponent*) i);
        } else if(IS_TYPE(Line, i)) {
            arrows.push_back((Line*) i);
        }
    }
}

std::map<UIComponent*, ComponentValue> AnalysisMapper::getSolution() {
    Graph graph = makeGraph();
	
    std::vector<Component> mNAComponents;

    // Map of UIComponent:Component to help to return the correct values to the correct UIComponent.
    QMap<UIComponent*, Component> mNAMap;

    foreach(UIComponent* uiComp, graph.keys()){
        Component* component;

        // Get the type of the node (node.first is node, node.second is connections).
        switch(uiComp->getId()){
            case UI_BATTERY:
                component = new Component(uiComp->n0, uiComp->n1, MNA_BATTERY, ((Battery*)uiComp)->getVoltage());
                break;

            // Resistors, wires and switches all have resistances.
            case UI_RESISTOR: case UI_WIRE: case UI_SWITCH: case UI_AMMETER: case UI_VOLTMETER:
                component = new Component(uiComp->n0, uiComp->n1, MNA_RESISTOR, ((ResistiveElement*)uiComp)->getResistance());
                break;

            default:
                continue;
        }

        // Add the new component to the list of components.
        mNAComponents.push_back(*component);

        // Add the component to the list to set the (UIComponent => Component).
        mNAMap.insert(uiComp, *component);
    }
	
    Circuit cir(mNAComponents);
	
    Solution sol = cir.solve();
	
    std::map<UIComponent*, ComponentValue> out;

	
    foreach(UIComponent* uiComp, mNAMap.keys()){
        Component comp = mNAMap[uiComp];
        switch(comp.type){
            // Resistors have a current and a voltage.
            case MNA_RESISTOR:
            	// Add the voltage and the current of the Component into the solution map
            	// with the key of the UIComponent.
                out[uiComp] = (ComponentValue){
                        sol.getVoltage(comp),
                        sol.getCurrent(comp)
                };
                break;

            // Batteries have only a voltage, not a current.
            case MNA_BATTERY:
            	// Add the voltage of the Component into the solution map.
            	// with the key of the UIComponent.
                out[uiComp] = (ComponentValue){
                        sol.getVoltage(comp),
                        NAN
                };
                break;

            default:
                break;
        }

        switch(uiComp->getId()){
            case UI_VOLTMETER:
                out[uiComp] = (ComponentValue){
                                sol.getVoltage(comp),
                    NAN
                        };
                        break;

            case UI_AMMETER:
                out[uiComp] = (ComponentValue){
                                NAN,
                    sol.getCurrent(comp)
                        };
                        break;

            default:
                break;
        }
    }
	
    return out;
}

Path* AnalysisMapper::findShortestPath(Graph *graph, UIComponent *start, UIComponent *end) {
    std::vector<UIComponent*> explored;

    std::queue<std::vector<UIComponent*>*> q;

    std::vector<UIComponent*>* v = new std::vector<UIComponent*>;
    v->push_back(start);
    q.push(v);

    // Is the path to the same node?
    // If so just return the end node.
    if(start == end){
        Path* out = new Path;
        out->push_back(end);
        return out;
    }

    while(! q.empty()){
        Path* path = q.front();
        q.pop();

        // Get end node from the path.
        UIComponent* node = path->at(path->size()-1);

        // If node has not already been explored
        if(std::find(explored.begin(), explored.end(), node) == explored.end()){
            std::vector<UIComponent*> neighbors = (*graph)[node];

            foreach(UIComponent* neighbor, neighbors){
                Path* new_path = new Path;
                std::copy(path->begin(), path->end(), back_inserter(*new_path));
                new_path->push_back(neighbor);
                q.push(new_path);

                // Have we reached the end node?
                // If so return the path.
                if(neighbor == end){
                    return new_path;
                }
            }

            explored.push_back(node);
        }
    }

    // If no path was found return an empty path.
    return new Path;
}

Graph AnalysisMapper::makeGraph() {
    // This is a map of UIComponent:list of UIComponents to show a matrix as an adjacency list.
    Graph graph;

    foreach(UIComponent* comp, components){
        std::vector<UIComponent*> connections;

        foreach(Line* a, comp->lines){
            // If the arrow goes to the component rather than away from it, ignore it.
            if(comp == a->endItem()){
                continue;
            }
            // Add a connection to the list.
            connections.push_back((UIComponent*) a->endItem());
        }

        // Add the list to the graph.
        graph[comp] = connections;
    }

    // TODO select correct start node
    UIComponent* start_node = components[0];

    foreach(UIComponent* uiComp, graph.keys()){
        // For each node set the node 0 to the distance from the start node.
        Path* path = findShortestPath(&graph, start_node, uiComp);

        uiComp->n0 = path->size()-1;

        // Temporarily set node 1 to 0.
        uiComp->n1 = 0;
    }

    foreach(UIComponent* uiComp, graph.keys()){
        uiComp->connections.clear();
        
        std::vector<UIComponent*> start = graph[uiComp];

        foreach(UIComponent* connectedComp, start){
            // For each connection to this node, set node 1 to the node 0 of the node it is connected to.
            uiComp->n1 = connectedComp->n0;
        }
    }

    return graph;
}
