//
// Created by rhys on 16/07/2021.
//

#include "CircuitSaver.h"
#include "../UI/AnalysisMapper.h"
#include "../UI/Components/Battery.h"
#include "../UI/Components/Resistor.h"
#include "../UI/Components/Wire.h"
#include "../UI/Components/Switch.h"
#include "FileUtils.h"

#include <iostream>
#include <QPainter>
#include <QBuffer>
#include <fstream>
#include <regex>

std::string CircuitSaver::getPath(std::string name){
    std::string saveDir = FileUtils::getSaveDir();

    // Concatenate the base save directory, the circuit name and the file extension.
    return saveDir + name + std::string(ext);
}

void CircuitSaver::saveCircuit(std::string name, SceneItems items) {
    // Get the JSON data from the serialiseCircuit function.
    std::string data = serialiseCircuit(name, items);

    std::string path = getPath(name);

    std::cout << "Save: '" << path << "'" << std::endl;

    // Save the JSON data to the file.
    std::ofstream file(path);
    file << data << std::endl;
    file.close();
}

void CircuitSaver::loadCircuit(std::string name, Scene* s) {
    std::string path = getPath(name);

    // Check if file exists, if not don't try to open it.
    if(not FileUtils::pathExists(path)){
        std::cerr << "Directory: '" << path << "' does not exist!" << std::endl;
        return;
    }

    std::cout << "Load: '" << path << "'" << std::endl;

    std::ifstream in(path);

    // Load the JSON string into a JSON object.
    json data;
    in >> data;
    in.close();

    std::map<int, UIComponent*> components;

    for(auto part : data["parts"]){
        UIComponent* comp = nullptr;

        // Create UIComponent for each json component.
        switch(part["component"]["type"].get<int>()) {
            case UI_BATTERY: {
                comp = new Battery(part["component"]["voltage"].get<double>(), part["component"]["state"].get<bool>());
                break;
            }
            case UI_RESISTOR: {
                comp = new Resistor(part["component"]["resistance"].get<double>());
                break;
            }
            case UI_WIRE: {
                comp = new Wire(part["component"]["length"].get<double>(), part["component"]["area"].get<double>(), part["component"]["material"].get<std::string>());
                break;
            }
            case UI_SWITCH: {
                comp = new Switch(part["component"]["state"].get<bool>());
                break;
            }

            default:
                break;
        }

        // If we actually made a component.
        if(comp != nullptr){
            // Set the coordinates of the component
            auto x = part["component"]["pos"][0].get<double>();
            auto y = part["component"]["pos"][1].get<double>();

            auto sceneSize = s->sceneRect();

            // Validate x, y minimum value of 0.
            x = x<0? 0 : x;
            y = y<0? 0 : y;

            // Validate x, y maximum value of the size of the scene.
            x = x>s->width? s->width : x;
            y = y>s->height? s->height : y;

            comp->setPos(x, y);

            // Get the ID of the component.
            comp->componentId = part["id"].get<int>();

            // Set the max ID by comparing this ID to the current maximum.
            if(comp->componentId > UIComponent::currentId){
                UIComponent::currentId = comp->componentId;
            }

            // Add the component to the scene.
            s->addItem(comp);
            // Add the component to the ID map.
            components[comp->componentId] = comp;
        }
    }

    for(auto part : data["parts"]){
        // Set the start item for all the arrows.
        auto startItem = components[part["id"].get<int>()];

        for(auto conn : part["connections"]){
            // For each connection in the JSON data get the component from the ID map.
            auto endItem = components[conn.get<int>()];

            // Create a line between the start and end components.
            auto* line = new Line(startItem, endItem);
            startItem->addLine(line);
            endItem->addLine(line);

            // Add the line to the scene and update it.
            s->addItem(line);
            line->update();
        }
    }

}

std::string CircuitSaver::serialiseCircuit(std::string name, SceneItems items) {
    json out = json::object();
    out["name"] = name;

    std::list<QGraphicsItem*> graphicsItems;

    // Cast each UIComponent into a QGraphicsItem for AnalysisMapper.
    std::transform(items.components.begin(), items.components.end(), std::back_inserter(graphicsItems), [](UIComponent* c){
        return (QGraphicsItem*) c;
    });

    AnalysisMapper am(graphicsItems);
    // Use AnalysisMapper to simplify the code to generate a graph of the components.
    Graph graph = am.makeGraph();

    json parts = json::array();

    for(auto it : graph){
        json sect = json::object();
        // Turn the component into JSON
        json comp = serialiseUIComponent(it.first);
        json connections = json::array();

        // For each connection add the ID of the connected component.
        for(auto c : it.second){
            connections.push_back(c->componentId);
        }

        // Add the data to the JSON map.
        sect["component"] = comp;
        sect["connections"] = connections;
        sect["id"] = it.first->componentId;

        parts.push_back(sect);
    }

    out["parts"] = parts;

    if(items.scene != nullptr){
        // Only create the image if we have a scene to convert.
        out["image"] = sceneToImage(items.scene);
    }

    return out.dump(4);
}

std::string CircuitSaver::sceneToImage(Scene *s, QImage::Format format) {
    // Create an image the size of the scene.
    auto image = QImage(s->itemsBoundingRect().size().toSize(), format);
    // Make the image white.
    image.fill(QColor(Qt::color0).rgb());

    QPainter painter;

    // Paint onto image.
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    // Render the scene onto the image.
    s->render(&painter, image.rect(), s->itemsBoundingRect());
    painter.end();

    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    // Save the image onto the buffer, which writes the data to the QByteArray.
    image.save(&buf, "PNG");

    // Convert the QByteArray to a base64 string and return.
    return ba.toBase64().toStdString();
}

json CircuitSaver::serialiseUIComponent(UIComponent* comp) {
    json out = json::object();

    // Get the type of component.
    out["type"] = comp->getId();

    // Different components have different saved values.
    switch(comp->getId()){
        case UI_BATTERY:
            // getVoltageValue as this gives the raw value, getVoltage changes the voltage by error
            // checking and including the state value.
            out["voltage"] = ((Battery*) comp)->getVoltageValue();
            out["state"] = ((Battery*) comp)->getState();
            break;
        case UI_RESISTOR:
            out["resistance"] = ((Resistor*) comp)->getResistance();
            break;
        case UI_WIRE:
            out["area"] = ((Wire*) comp)->getArea();
            out["length"] = ((Wire*) comp)->getLength();
            out["material"] = ((Wire*) comp)->getMaterial();
            break;
        case UI_SWITCH:
            out["state"] = ((Switch*) comp)->getState();
            break;
    }

    // Add the coordinates of the component in a list of [x,y].
    out["pos"] = json::array({comp->pos().x(), comp->pos().y()});

    return out;
}

void CircuitSaver::exportCircuit(std::string name, std::string path) {
    auto saveFile = getPath(name);

    // If there is no path to export, quit.
    if(path.empty()){
        return;
    }

    // Append file extension if not there.
    if(! FileUtils::endsWith(path, ext)){
        path += ext;
    }

    // Copy file to export directory.
    FileUtils::copyFile(saveFile, path);
}

void CircuitSaver::importCircuit(std::string path) {
    // Regex removes path and extracts file name, e.g. "/home/rhys/test.cir", the output is an array ["/home/rhys/test.cir", "test"]
    std::regex nameRegex(R"(.*(?:\/|\\)(.*).cir)");
    std::cmatch match;

    // Search the string to match the regex above.
    if(! std::regex_search(path.c_str(), match, nameRegex)){
        std::cerr << "Cannot regex match: " << path << std::endl;
        return;
    }

    // Match[1] is the 1st match group which is the file name.
    if(match[1] != ""){
        auto saveFile = CircuitSaver::getPath(match[1]);
        FileUtils::copyFile(path, saveFile);
    }
}
