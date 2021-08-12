//
// Created by rhys on 16/07/2021.
//

#include "CircuitSaver.h"
#include "../UI/AnalysisMapper.h"
#include "../UI/Components/Battery.h"
#include "../UI/Components/Resistor.h"
#include "../UI/Components/Wire.h"
#include "../UI/Components/Switch.h"

#include <iostream>
#include <QPainter>
#include <QBuffer>


void CircuitSaver::saveCircuit(std::string name, std::vector<UIComponent *> components, std::vector<Arrow *> arrows, Scene* s) {
    std::string data = serialiseCircuit(name, components, arrows, s);
}

std::string CircuitSaver::serialiseCircuit(std::string name, std::vector<UIComponent *> components, std::vector<Arrow *> arrows, Scene* s) {
    json out = json::object();
    out["name"] = name;

    std::list<QGraphicsItem*> gi;
    std::transform(components.begin(), components.end(), std::back_inserter(gi), [](UIComponent* c){
        return (QGraphicsItem*) c;
    });

    AnalysisMapper am(gi);
    Graph g = am.makeGraph();

    json parts = json::array();

    for(auto i : g){
        json sect = json::object();
        json comp = serialiseUIComponent(i.first);
        json connections = json::array();
        for(auto c : i.second){
            connections.push_back({c->pos().x(), c->pos().y()});
        }
        sect["component"] = comp;
        sect["connections"] = connections;

        parts.push_back(sect);
    }

    out["parts"] = parts;

    if(s != nullptr){
        out["image"] = sceneToImage(s);
    }

    return out.dump();
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
            out["state"] = ((Switch*) comp)->enabledButton->isChecked();
            break;
    }

    return out;
}
