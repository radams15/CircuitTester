//
// Created by rhys on 16/07/2021.
//

#ifndef CIRCUITTESTER_CIRCUITSAVER_H
#define CIRCUITTESTER_CIRCUITSAVER_H

#include <QtCore/QList>
#include <QtGui/QGraphicsItem>
#include "../UI/UIComponent.h"
#include "../UI/Line.h"
#include "../UI/Scene.h"

#include "json.h"

/** @brief Items in the scene.
 *
 */
struct SceneItems{
    /** @brief List of components on the scene.
     *
     */
    std::vector<UIComponent*> components;

    /** @brief List of lines on the scene.
     *
     */
    std::vector<Line*> lines;
};

#define ext ".cir"

/** @brief Class that saves/loads/exports/imports circuits.
 *
 */
class CircuitSaver {
private:

    /** @brief Convert a UIComponent into JSON data.
     *
     * @param comp The component to convert.
     * @return JSON data that corresponds to the component.
     */
    static json::jobject serialiseUIComponent(UIComponent* comp);

public:
    /** @brief Gets the save path from the circuit name.
     *
     * @param name The name of the circuit.
     * @return The path to save/load the circuit from.
     */
    static std::string getPath(std::string name);

    /** @brief Save circuit to file.
     *
     * @param name The name of the circuit.
     * @param items The SceneItems object containing the components and lines.
     */
    static void saveCircuit(std::string name, SceneItems items);

    /** @brief Converts circuit to string.
     *
     * @param name The name of the circuit.
     * @param items The SceneItems object containing the components and lines.
     * @return JSON string of circuit.
     */
    static std::string serialiseCircuit(std::string name, SceneItems items);

    /** Load circuit onto scene.
     *
     * @param name Name of the circuit to load.
     * @param s Scene pointer to write the components to.
     */
    static void loadCircuit(std::string name, Scene* s);

    /** @brief Copy circuit to directory to send to others.
     *
     * @param name Name of circuit to export.
     * @param path Path to export to.
     */
    static void exportCircuit(std::string name, std::string path);

    /** @brief Import circuit to library.
     *
     * @param path Path to import into library.
     */
    static void importCircuit(std::string path);
};

#endif //CIRCUITTESTER_CIRCUITSAVER_H
