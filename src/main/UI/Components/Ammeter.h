/** @brief
 * @author Rhys Adams
 * @date 08/04/2021
 */

#ifndef LAYOUTTEST1_AMMETER_H
#define LAYOUTTEST1_AMMETER_H

#include "ResistiveElement.h"

/** @brief Ammeter component.
 *
 */
class Ammeter : public ResistiveElement {
private:

public:
    enum{
        /** @brief Set ID to resistor.
         *
         */
        ID = UI_AMMETER
    };

    /** @brief Initialiser.
     *
     */
    Ammeter();

    /** @brief Overrides the value in ResistiveElement to return min resistance to be a perfect ammeter.
     *
     * @return Resistance of the component in Ohms.
     */
    double getResistance() override;

    YAML::Node toYaml();
};


#endif //LAYOUTTEST1_AMMETER_H
