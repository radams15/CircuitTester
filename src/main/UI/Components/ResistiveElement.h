/** @brief
 * @author Rhys Adams
 * @date 12/08/2021
 */

#ifndef CIRCUITTESTER_RESISTIVEELEMENT_H
#define CIRCUITTESTER_RESISTIVEELEMENT_H

#include <utility>

#include "../UIComponent.h"

/** @brief Parent class of anything with a resistance (Resistor, Switch, Wire).
 *
 */
class ResistiveElement : public UIComponent {
public:
    /** @brief Initialiser.
     *
     * @param ID Type of the component.
     * @param resourcePath Path to the image of the component.
     */
    ResistiveElement(int ID, std::string resourcePath) : UIComponent(ID, std::move(resourcePath)){}

    /** @brief Virtual method for getting the resistance of a resistive component.
     *
     * @return
     */
    virtual double getResistance() = 0;
};

#endif //CIRCUITTESTER_RESISTIVEELEMENT_H
