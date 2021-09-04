/** @brief
 * @author Rhys Adams
 * @date 10/08/2021
 */

#ifndef LAYOUTTEST1_WIRE_H
#define LAYOUTTEST1_WIRE_H


#include <map>
#include <string>

#include <QDoubleSpinBox>
#include <QComboBox>
#include "ResistiveElement.h"

/** @brief Wire component with configurable length, area and material.
 *
 */
class Wire : public ResistiveElement {
private:
    /** @brief Map of material name: resistivity in ohm metres.
     *
     */
    std::map<std::string, long double> resistivities = {
            {"Copper", 1.68E-8},
            {"Iron", 1.0E-8},
            {"Lead", 2.2E-8},
            {"Carbon", 6E-4},
    };

    /** @brief Spinner for length in cm.
     *
     */
    QDoubleSpinBox* lengthSpinner;

    /** @brief Spinner for area in m^2.
     *
     */
    QDoubleSpinBox* areaSpinner;

    /** @brief Spinner for wire material.
     *
     */
    QComboBox* wireCombo;

public:
    enum{
        /** @brief Set ID to wire.
         *
         */
        ID = UI_WIRE
    };

    /** @brief Initialiser.
     *
     * @param length Length of the wire in cm (Default 1cm).
     * @param area Area of the wire in mm^2 (Default 1mm^2).
     * @param material Material of the wire (Default copper).
     */
    Wire(double length=1.0f, double area=1.0f, std::string material="Copper");

    /** @brief Overrides the value in ResistiveElement to return the value of resistanceSpinner.
     *
     * @return The resistance of the wire using p=(RA/l).
     */
    double getResistance() override;

    /** @brief Gets the wire length.
     *
     * @return Length of the wire in cm.
     */
    inline double getLength() const{ return lengthSpinner->value(); }

    /** @brief Gets the wire area.
     *
     * @return Area of wire in mm^2.
     */
    inline double getArea() const{ return areaSpinner->value(); }

    /** @brief Gets the material.
     *
     * @return Material of the wire as string value.
     */
    inline std::string getMaterial() const{ return wireCombo->currentText().toStdString(); }
};


#endif //LAYOUTTEST1_WIRE_H
