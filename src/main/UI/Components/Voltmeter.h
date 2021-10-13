/** @brief
 * @author Rhys Adams
 * @date 08/04/2021
 */

#ifndef LAYOUTTEST1_VOLTMETER_H
#define LAYOUTTEST1_VOLTMETER_H

#include <QDoubleSpinBox>
#include "ResistiveElement.h"

/** @brief Resistor component with configurable resistance.
 *
 */
class Voltmeter : public ResistiveElement {
private:

public:
    enum{
        /** @brief Set ID to resistor.
         *
         */
        ID = UI_VOLTMETER
    };

    /** @brief Initialiser.
     *
     */
    Voltmeter();

    /** @brief Overrides the value in ResistiveElement to return max resistance to be a perfect voltmeter.
     *
     * @return Resistance of the component in Ohms.
     */
    double getResistance() override;
};


#endif //LAYOUTTEST1_VOLTMETER_H
