//
// Created by rhys on 15/05/2021.
//

#ifndef CIRCUITTESTER_ANALYSISMAPPER_H
#define CIRCUITTESTER_ANALYSISMAPPER_H

#include <vector>
#include <list>

#include "SceneItem.h"

#include "../Analysis/MNASolution.h"
#include "UIComponent.h"

typedef std::map<UIComponent*, std::vector<UIComponent*>> Graph;
typedef std::vector<UIComponent*> Path;

class AnalysisMapper {
private:
    std::vector<UIComponent*> components;
    std::vector<Arrow*> arrows;

    Path* find_shortest_path(Graph* graph, UIComponent* start, UIComponent* end);

    Graph makeGraph();

public:
    explicit AnalysisMapper(std::list<QGraphicsItem*> graphicsItems);

    MNASolution* getSolution();
};


#endif //CIRCUITTESTER_ANALYSISMAPPER_H
