/** @brief
 * @author Rhys Adams
 * @date 21/04/2021
 */

#ifndef LAYOUTTEST1_BATTERY_H
#define LAYOUTTEST1_BATTERY_H


#include <QDoubleSpinBox>
#include <QCheckBox>
#include "../UIComponent.h"

/** @brief A battery class that has a configurable voltage
 * and on/off state.
 *
 */
class Battery : public UIComponent {

private:
    /** @brief Spinbox to set voltage.
     *
     */
    QDoubleSpinBox* voltageSpinner;

    /** @brief Checkbox to set on/off state.
     *
     */
    QCheckBox* onOffCheckbox;

public:
    enum{
        /** @brief Set the ID to a battery.
         *
         */
        ID = UI_BATTERY
    };

    /** @brief Gets the voltage, but takes into account the
     * state.
     *
     * @return Voltage or 0.0001 if battery is turned off.
     */
    double getVoltage();

    /** @brief Initialiser.
     *
     * @param voltage Voltage to set the battery to in volts (default 1V).
     * @param on Whether the battery is on/off (default on).
     */
    Battery(double voltage=1.0f, bool on=true);

    /** @brief Returns the direct value of the voltage spinner for CircuitSaver.
     *
     * @return The value of voltageSpinner.
     */
    inline double getVoltageValue(){ return voltageSpinner->value(); }

    /** @brief Returns the on/off state of the battery.
     *
     * @return Whether the on/off box is checked or not.
     */
    inline bool getState(){ return onOffCheckbox->isChecked(); }

    json::jobject toJson();
};


#endif //LAYOUTTEST1_BATTERY_H
