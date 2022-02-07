/** @brief
 * @author Rhys Adams
 * @date 10/08/2021
 */

#ifndef LAYOUTTEST1_SWITCH_H
#define LAYOUTTEST1_SWITCH_H

#include <QDoubleSpinBox>
#include <QCheckBox>
#include "ResistiveElement.h"

/** @brief Switch component that can be on/off.
 *
 */
class Switch : public ResistiveElement {
private:

    /** @brief The resistance when the switch is off is miniscule so that current can flow.
     *
     */
    const double switchOn = 0.000001;

    /** @brief The resistance when the switch is on is very large so that current cannot flow.
     *
     */
    const double switchOff = 10000000;

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
    double getResistance() override;

    /** @brief Gets the state of the switch.
     *
     * @return Whether the on/off checkbox is checked or not.
     */
    inline bool getState(){ return onOffCheckbox->isChecked(); }

    json::jobject toJson();
};


#endif //LAYOUTTEST1_SWITCH_H
