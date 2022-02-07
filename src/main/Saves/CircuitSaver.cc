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
#include <fstream>
#include <sstream>

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
    std::ofstream file(path.c_str());
    file << data << std::endl;
    file.close();
}

json::jobject get(json::jobject root, std::string key){
	return json::jobject::parse(root.get(key));
}

void CircuitSaver::loadCircuit(std::string name, Scene* s) {
    std::string path = getPath(name);

    // Check if file exists, if not don't try to open it.
    if(not FileUtils::pathExists(path)){
        std::cerr << "Directory: '" << path << "' does not exist!" << std::endl;
        return;
    }

    /*std::cout << "Load: '" << path << "'" << std::endl;

    Table data = cpptoml::parse_file(path.c_str());

    std::map<int, UIComponent*> components;

	Array parts = data->get_as<Array>("parts").value_or(cpptoml::make_array());
	
    for(int i=0 ; i<parts->size() ; i++){
		json::jobject component = parts[i]["component"];
		int uid = parts[i]["id"];
        UIComponent* comp = NULL;
		
		int id = component["type"];
				
        // Create UIComponent for each json component.
        switch(id) {
            case UI_BATTERY: {
                comp = (UIComponent*) new Battery((double) component["voltage"], (int) component["state"]);
                break;
            }
            case UI_RESISTOR: {
                comp = (UIComponent*) new Resistor((double) component["resistance"]);
                break;
            }
            case UI_WIRE: {
                comp = (UIComponent*) new Wire((double) component["length"], (double) component["area"], component["material"]);
                break;
            }
            case UI_SWITCH: {
                comp = (UIComponent*) new Switch((int) component["state"]);
                break;
            }

            default:
                break;
        }

        // If we actually made a component.
        if(comp != NULL){
            // Set the coordinates of the component
            double x = (double) component["pos"].array(0);
            double y = (double) component["pos"].array(1);
			
            // Validate x, y minimum value of 0.
            x = x<0? 0 : x;
            y = y<0? 0 : y;

            // Validate x, y maximum value of the size of the scene.
            x = x>s->width? s->width : x;
            y = y>s->height? s->height : y;

            comp->setPos(x, y);

            // Get the ID of the component.
            comp->componentId = uid;

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
	
	parts = data["parts"];
	
    for(int i=0 ; i<parts.size() ; i++){
		json::jobject part = parts[i];
        // Set the start item for all the arrows.
        UIComponent* startItem = components[(int)part["id"]];

		
		std::vector<int> connections = part["connections"];
        for(int x=0 ; x<connections.size() ; x++){
			int conn = connections[x];
            // For each connection in the JSON data get the component from the ID map.
            UIComponent* endItem = components[conn];

            // Create a line between the start and end components.
            Line* line = new Line(startItem, endItem);
            startItem->addLine(line);
            endItem->addLine(line);

            // Add the line to the scene and update it.
            s->addItem(line);
            line->redraw();
        }
    }*/

}

std::string CircuitSaver::serialiseCircuit(std::string name, SceneItems items) {
	Table out;
    out->insert("name", name);

    std::list<QGraphicsItem*> graphicsItems(items.components.begin(), items.components.end());

    AnalysisMapper am(graphicsItems);
    // Use AnalysisMapper to simplify the code to generate a graph of the components.
    Graph graph = am.makeGraph();

    Table parts = cpptoml::make_table();

    int i=0;
    foreach(UIComponent* uicomp, graph.keys()){

        Table sect = cpptoml::make_table();
        // Turn the component into JSON
        Table comp = serialiseUIComponent(uicomp);
		Array connections = cpptoml::make_array();
		
		std::vector<UIComponent*> components = graph[uicomp];

        // For each connection add the ID of the connected component.
        for(int i=0 ; i<components.size() ; i++){
            connections->push_back(components[i]->componentId);
        }

        // Add the data to the JSON map.
        sect->insert("component", comp);
        sect->insert("connections", connections);
        sect->insert("id", uicomp->componentId);

        parts->insert(std::to_string(i), sect);
    }

    out->insert("parts", parts);

    std::stringstream outs;
    outs << (*out);

    return outs.str();
}


Table CircuitSaver::serialiseUIComponent(UIComponent* comp) {
    Table out = cpptoml::make_table();

    // Get the type of component.
    out->insert("type", comp->getId());

    // Different components have different saved values.
    switch(comp->getId()){
        case UI_BATTERY:
            // getVoltageValue as this gives the raw value, getVoltage changes the voltage by error
            // checking and including the state value.
            out->insert("voltage", ((Battery*) comp)->getVoltageValue());
            out->insert("state", ((Battery*) comp)->getState());
            break;
        case UI_RESISTOR:
            out->insert("resistance", ((Resistor*) comp)->getResistance());
            break;
        case UI_WIRE:
            out->insert("area", ((Wire*) comp)->getArea());
            out->insert("length", ((Wire*) comp)->getLength());
            out->insert("material", ((Wire*) comp)->getMaterial());
            break;
        case UI_SWITCH:
            out->insert("state", ((Switch*) comp)->getState());
            break;
    }

    // Add the coordinates of the component in a list of [x,y].
	Array pos = cpptoml::make_array();
	pos->push_back(comp->pos().x());
	pos->push_back(comp->pos().y());
    out->insert("pos", pos);

    return out;
}

void CircuitSaver::exportCircuit(std::string name, std::string path) {
    std::string saveFile = getPath(name);

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
	std::string saveFile = CircuitSaver::getPath(path);
	FileUtils::copyFile(path, saveFile);
}
