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
#include "Arrow.h"

#include "../Analysis/MNACircuit.h"

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

std::map<UIComponent*, ComponentValue> AnalysisMapper::getSolution() {
    Graph graph = makeGraph();

    auto* MNAComponents = new std::vector<MNAComponent*>;
    auto* MNAMap = new std::map<UIComponent*, MNAComponent*>;

    for(auto n : graph){
        MNAComponent* c;
        if(n.first->getId() == UI_BATTERY){
            c = new MNAComponent(n.first->n0, n.first->n1, MNA_BATTERY, ((Battery*)n.first)->getVoltage());
            MNAComponents->push_back(c);
        }else if(n.first->getId() == UI_RESISTOR){
            c = new MNAComponent(n.first->n0, n.first->n1, MNA_RESISTOR, ((Resistor*)n.first)->getResistance());
            MNAComponents->push_back(c);
        }else if(n.first->getId() == UI_WIRE){
            c = new MNAComponent(n.first->n0, n.first->n1, MNA_RESISTOR, ((Wire*)n.first)->getResistance());
            MNAComponents->push_back(c);
        }else if(n.first->getId() == UI_SWITCH){
            c = new MNAComponent(n.first->n0, n.first->n1, MNA_RESISTOR, ((Switch*)n.first)->getEnabled() ? 0.000001 : 1000000);
            MNAComponents->push_back(c);
        }else{
            continue;
        }

        MNAMap->insert(std::make_pair(n.first, c));
    }

    auto* cir = new MNACircuit(*MNAComponents);

    auto* sol = cir->solve();

    std::map<UIComponent*, ComponentValue> out;

    for(auto c : *MNAMap){
        switch(c.second->type){
            case MNA_RESISTOR:
                out[c.first] = {
                        sol->getVoltage(*c.second),
                        sol->getCurrent(*c.second)
                };
                break;

            case MNA_BATTERY:
                out[c.first] = {
                        sol->getVoltage(*c.second),
                        NAN
                };
                break;

            default:
                break;
        }
    }

    return out;
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

        //std::cout << n.first->n0 << "(" << n.first->getId() << ") => " << n.first->n1 << std::endl;
    }

    return graph;
}
