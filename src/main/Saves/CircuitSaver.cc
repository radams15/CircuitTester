//
// Created by rhys on 16/07/2021.
//

#include "CircuitSaver.h"
#include "../UI/AnalysisMapper.h"
#include "../UI/Components/Battery.h"
#include "../UI/Components/Resistor.h"
#include "../UI/Components/Wire.h"
#include "../UI/Components/Switch.h"
#include "UserUtils.h"

#include <iostream>
#include <QPainter>
#include <QBuffer>
#include <fstream>

const std::string CircuitSaver::ext = ".cir";

std::string CircuitSaver::getPath(std::string name){
    std::string saveDir = UserUtils::getSaveDir();

    return saveDir + name + ext;
}

void CircuitSaver::saveCircuit(std::string name, SceneItems items) {
    std::string data = serialiseCircuit(name, items);

    std::string path = getPath(name);

    std::cout << "Save to: " << path << std::endl;

    std::ofstream file(path);
    file << data << std::endl;
    file.close();
}

void CircuitSaver::loadCircuit(std::string name, Scene* s) {
    std::string path = getPath(name);

    std::cout << "Load: " << path << std::endl;

    std::ifstream in(path);

    json data;
    in >> data;
    in.close();

    std::map<int, UIComponent*> components;

    for(auto part : data["parts"]){
        UIComponent* comp = nullptr;

        switch(part["component"]["type"].get<int>()){ // TODO validate all this
            case UI_BATTERY: {
                auto b = new Battery;
                b->voltageSpinner->setValue(part["component"]["voltage"].get<double>());
                b->onOffCheckbox->setChecked(part["component"]["state"].get<bool>());
                comp = (UIComponent*) b;
                break;
            }
            case UI_RESISTOR: {
                auto r = new Resistor;
                r->resistanceSpinner->setValue(part["component"]["resistance"].get<double>());
                comp = (UIComponent*) r;
                break;
            }
            case UI_WIRE: {
                auto w = new Wire;
                w->areaSpinner->setValue(part["component"]["area"].get<double>());
                w->lengthSpinner->setValue(part["component"]["length"].get<double>());
                w->wireCombo->setCurrentText(QString::fromStdString(part["component"]["material"].get<std::string>()));
                comp = (UIComponent*) w;
                break;
            }
            case UI_SWITCH: {
                auto sw = new Switch;
                sw->onOffCheckbox->setChecked(part["component"]["state"].get<bool>());
                comp = (UIComponent*) sw;
                break;
            }
        }

        if(comp != nullptr){
            comp->setPos(part["component"]["pos"][0].get<double>(), part["component"]["pos"][1].get<double>());
            comp->componentId = part["id"].get<int>();
            if(comp->componentId > UIComponent::currentId){
                UIComponent::currentId = comp->componentId;
            }
            s->addItem(comp);
            components[comp->componentId] = comp;
        }
    }

    for(auto part : data["parts"]){
        auto startItem = components[part["id"].get<int>()];

        for(auto conn : part["connections"]){
            auto endItem = components[conn.get<int>()];

            std::cout << startItem->componentId << " => " << endItem->componentId << std::endl;

            auto* line = new Line(startItem, endItem);
            startItem->addArrow(line);
            endItem->addArrow(line);
            s->addItem(line);
            line->update();
        }
    }

}

std::string CircuitSaver::serialiseCircuit(std::string name, SceneItems items) {
    json out = json::object();
    out["name"] = name;

    std::list<QGraphicsItem*> graphicsItems;
    std::transform(items.components.begin(), items.components.end(), std::back_inserter(graphicsItems), [](UIComponent* c){
        return (QGraphicsItem*) c;
    });

    AnalysisMapper am(graphicsItems);
    Graph g = am.makeGraph();

    json parts = json::array();

    for(auto i : g){
        json sect = json::object();
        json comp = serialiseUIComponent(i.first);
        json connections = json::array();
        for(auto c : i.second){
            connections.push_back(c->componentId);
        }
        sect["component"] = comp;
        sect["connections"] = connections;
        sect["id"] = i.first->componentId;

        parts.push_back(sect);
    }

    out["parts"] = parts;

    if(items.scene != nullptr){
        out["image"] = sceneToImage(items.scene);
    }

    return out.dump(4);
}

std::string CircuitSaver::sceneToImage(Scene *s, QImage::Format format) {
    auto image = QImage(s->itemsBoundingRect().size().toSize(), format);
    image.fill(QColor(Qt::color0).rgb());

    QPainter painter;

    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    s->render(&painter, image.rect(), s->itemsBoundingRect());
    painter.end();

    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf, "PNG");

    return ba.toBase64().toStdString();
}

json CircuitSaver::serialiseUIComponent(UIComponent* comp) {
    json out = json::object();
    out["type"] = comp->getId();

    switch(comp->getId()){
        case UI_BATTERY:
            out["voltage"] = ((Battery*) comp)->voltageSpinner->value();
            out["state"] = ((Battery*) comp)->onOffCheckbox->isChecked();
            break;
        case UI_RESISTOR:
            out["resistance"] = ((Resistor*) comp)->resistanceSpinner->value();
            break;
        case UI_WIRE:
            out["area"] = ((Wire*) comp)->areaSpinner->value();
            out["length"] = ((Wire*) comp)->lengthSpinner->value();
            out["material"] = ((Wire*) comp)->wireCombo->currentText().toStdString();
            break;
        case UI_SWITCH:
            out["state"] = ((Switch*) comp)->onOffCheckbox->isChecked();
            break;
    }

    out["pos"] = json::array({comp->pos().x(), comp->pos().y()});

    return out;
}
