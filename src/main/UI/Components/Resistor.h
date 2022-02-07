/** @brief
 * @author Rhys Adams
 * @date 08/04/2021
 */

#ifndef LAYOUTTEST1_RESISTOR_H
#define LAYOUTTEST1_RESISTOR_H

#include <QDoubleSpinBox>
#include "ResistiveElement.h"

/** @brief Resistor component with configurable resistance.
 *
 */
class Resistor : public ResistiveElement {
private:
    /** Spinbox to set resistance.
     *
     */
    QDoubleSpinBox* resistanceSpinner;
public:
    enum{
        /** @brief Set ID to resistor.
         *
         */
        ID = UI_RESISTOR
    };

    /** @brief Initialiser.
     *
     * @param resistance Set resistance of the resistor in Ohms (Default 1 Ohm)
     */
    Resistor(double resistance=1.0f);

    /** @brief Overrides the value in ResistiveElement to return the value of resistanceSpinner.
     *
     * @return Resistance of the component in Ohms.
     */
    double getResistance() override;

    json::jobject toJson();
};


#endif //LAYOUTTEST1_RESISTOR_H
