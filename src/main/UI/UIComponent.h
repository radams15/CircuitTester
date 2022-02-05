/** @brief
 * @author Rhys Adams
 * @date 15/05/2021
 */

#ifndef CIRCUITTESTER_UICOMPONENT_H
#define CIRCUITTESTER_UICOMPONENT_H

#include <string>

#include <utility>
#include <QVBoxLayout>
#include <cfloat>

#include "SceneItem.h"

/** @brief Enum to determine the type of UI component is in the
 * generic UIComponent.
 *
 * Must be numbered so that when saved they can be re-loaded with
 * the same values.
 *
 */
enum UIComponentType{
    UI_RESISTOR = 1,
    UI_BATTERY = 2,
    UI_WIRE = 3,
    UI_SWITCH = 4,
    UI_VOLTMETER = 5,
    UI_AMMETER = 6,
};

/** @brief SceneItem derivative that has information about generic component
 * info such as nodes and arrow connections.
 *
 */
class UIComponent : public SceneItem{
private:

    /** @brief The Colour when on (green)
     *
     */
    QColor onColor = {0,255,0};

    /** @brief The Colour when off (black)
    *
    */
    QColor offColor = {0,0,0};

protected:
    /** @brief Is type UIComponentType, allows other functions to find what type of component
     * pointers are.
     *
     */
    int ID;

    /** @brief Protected initialiser as we don't want to be able to
     * initialise a bare UIComponent, only its derivatives.
     *
     * @param id The type of component.
     * @param resourcePath The path to the image of the component.
     */
    UIComponent(int id, std::string resourcePath);

public:
    /** @brief Getter for ID.
     *
     * @return The component ID.
     */
    inline int getId() const { return ID; }

    /** @brief Sets whether or not the component is deemed on or off.
     *
     * @param on Boolean: is component on?
     */
    void setState(bool on);

    /** @brief The constantly incrementing id field which is used to differentiate between
     * individual unique components. This is only used in CircuitSaver.
     *
     */
    static int currentId;

    /** @brief The id of the component, this does not change but is to show connections
     * in save files, e.g. component 1 connects to component 2.
     *
     */
    int componentId;

    /** @brief A container that holds the settings for each component.
     *
     */
    QVBoxLayout* settingsBox;

    /** @brief Stores the start node for conversion to MNAComponents.
     *
     * Default -1 as unset.
     *
     */
    int n0 = -1;

    /** @brief Stores the end node for conversion to MNAComponents.
     *
     * Default -1 as unset.
     *
     */
    int n1 = -1;

    /** @brief List of lines that are connected to this component.
     *
     */
    std::vector<Line*> connections;
};


#endif //CIRCUITTESTER_UICOMPONENT_H
