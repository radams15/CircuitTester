/** @brief
 * @author Rhys Adams
 * @date 15/05/2021
 */

#ifndef CIRCUITTESTER_UICOMPONENT_H
#define CIRCUITTESTER_UICOMPONENT_H

#include <string>

#include <utility>
#include <QVBoxLayout>

#include "SceneItem.h"

/** @brief Enum to determine the type of UI component is in the
 * generic UIComponent.
 *
 */
enum ComponentType{
    UI_RESISTOR,
    UI_BATTERY,
    UI_WIRE,
    UI_SWITCH
};

/** @brief SceneItem derivative that has information about generic component
 * info such as nodes and arrow connections.
 *
 */
class UIComponent : public SceneItem{
protected:
    /** @brief ComponentType
     *
     */
    int ID;

    UIComponent(int id, std::string resourcePath);

public:
    inline int getId() const { return ID; }

    QVBoxLayout* settingsBox;

    int n0 = -1;
    int n1 = -1;

    std::vector<Arrow*> connections;

    bool equals(UIComponent* c);
};


#endif //CIRCUITTESTER_UICOMPONENT_H
