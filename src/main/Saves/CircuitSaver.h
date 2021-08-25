//
// Created by rhys on 16/07/2021.
//

#ifndef CIRCUITTESTER_CIRCUITSAVER_H
#define CIRCUITTESTER_CIRCUITSAVER_H

#include <QList>
#include <QGraphicsItem>
#include "../UI/UIComponent.h"
#include "../UI/Arrow.h"
#include "../UI/Scene.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct SceneItems{
    std::vector<UIComponent*> components;
    std::vector<Arrow*> arrows;
    Scene* scene = nullptr;
};

class CircuitSaver {
private:

    static std::string sceneToImage(Scene *s, QImage::Format format=QImage::Format_RGB16);

    static json serialiseUIComponent(UIComponent* comp);

    static std::string getPath(std::string name);

public:
    static const std::string ext;

    static std::string serialiseCircuit(std::string name, SceneItems items);
    static void saveCircuit(std::string name, SceneItems items);

    static void loadCircuit(std::string name, Scene* s);
};

#endif //CIRCUITTESTER_CIRCUITSAVER_H
