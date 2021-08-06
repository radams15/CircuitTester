//
// Created by rhys on 15/05/2021.
//

#include <queue>
#include "AnalysisMapper.h"

#include "UIComponent.h"
#include "Components/Battery.h"
#include "Components/Resistor.h"
#include "Arrow.h"

#include <iostream>

AnalysisMapper::AnalysisMapper(std::list<QGraphicsItem*> graphicsItems) {
    for(QGraphicsItem *i : graphicsItems){
        if(IS_TYPE(UIComponent, i)) {
            components.push_back((UIComponent*) i);
        } else if(IS_TYPE(Arrow, i)) {
            arrows.push_back((Arrow*) i);
        }
    }
}

MNASolution* AnalysisMapper::getSolution() {
    Graph g = makeGraph();

    return nullptr;
}

Path* AnalysisMapper::find_shortest_path(Graph *graph, UIComponent *start, UIComponent *end) {
    std::vector<UIComponent*> explored;

    std::queue<std::vector<UIComponent*>*> q;

    auto v = new std::vector<UIComponent*>;
    v->push_back(start);
    q.emplace(v);

    if(start == end){
        auto out = new Path;
        out->push_back(end);
        return out;
    }

    while(! q.empty()){
        Path* path = q.front();
        q.pop();

        UIComponent* node = path->at(path->size()-1);

        if(std::find(explored.begin(), explored.end(), node) == explored.end()){
            auto neighbors = graph->at(node);

            for(auto neighbor : neighbors){
                Path* new_path = new Path;
                std::copy(path->begin(), path->end(), back_inserter(*new_path));
                new_path->push_back(neighbor);
                q.emplace(new_path);

                if(neighbor == end){
                    return new_path;
                }
            }

            explored.push_back(node);
        }
    }

    return new Path;
}

Graph AnalysisMapper::makeGraph() {
    Graph graph;

    for(auto c : components){
        std::vector<UIComponent*> connections;
        if(c->getId() == UI_BATTERY){
            std::cout << ((Battery*) c)->getVoltage() << "V\n";
        }

        for(auto a : c->arrows){
            if(c == a->endItem()){
                continue;
            }
            connections.push_back((UIComponent*) a->endItem());
        }

        graph[c] = connections;
    }

    auto start_node = components[0];

    for(auto n : graph){
        auto* path = find_shortest_path(&graph, start_node, n.first);

        n.first->n0 = path->size()-1;
        n.first->n1 = 0;
    }


    for(auto n : graph){
        n.first->connections.clear();
        for(auto c : graph.at(n.first)){
            n.first->n1 = c->n0;
        }

        std::cout << n.first->n0 << "(" << n.first->getId() << ") => " << n.first->n1 << std::endl;
    }

    return graph;
}
