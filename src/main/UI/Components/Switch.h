/** @brief
 * @author Rhys Adams
 * @date 10/08/2021
 */

#ifndef LAYOUTTEST1_SWITCH_H
#define LAYOUTTEST1_SWITCH_H

#include <QtGui/QDoubleSpinBox>
#include <QtGui/QCheckBox>
#include "ResistiveElement.h"

#define switchOn 0.000001
#define switchOff 10000000

/** @brief Switch component that can be on/off.
 *
 */
class Switch : public ResistiveElement {
private:
    /** @brief Checkbox to show on/off. Checked=on.
     *
     */
    QCheckBox* onOffCheckbox;

public:

    enum{
        /** @brief Set the ID to switch.
         *
         */
        ID = UI_SWITCH
    };

    /** @brief Initialiser.
     *
     * @param on Whether the switch is on/off (Default on).
     */
    Switch(bool on=true);

    /** @brief Overrides the value in ResistiveElement to return the value of resistanceSpinner.
     *
     * @return The resistance of the switch in ohms, high resistance if off, low resistance if on.
     */
    double getResistance();

    /** @brief Gets the state of the switch.
     *
     * @return Whether the on/off checkbox is checked or not.
     */
    inline bool getState(){ return onOffCheckbox->isChecked(); }
};


#endif //LAYOUTTEST1_SWITCH_H
