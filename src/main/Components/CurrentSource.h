/** @brief Current Source class derived from MNAElement.
 * @author Rhys Adams
 * @date 08/05/2021
 */

#ifndef CIRCUITTESTER_CURRENTSOURCE_H
#define CIRCUITTESTER_CURRENTSOURCE_H

#include "../Analysis/MNAElement.h"

class CurrentSource : public MNAElement{
public:
    /** @brief Initialiser
     *
     * The same as from MNAElement, but type is automatically set to CURRENT_SRC.
     *
     * @param n0 The start node
     * @param n1 The end node
     * @param value The value of this component
     * @param currentSolution The current solution if needed, otherwise it is just NAN
     */
    CurrentSource(int n0, int n1, double value, double currentSolution=NAN);
};

#endif //CIRCUITTESTER_CURRENTSOURCE_H
