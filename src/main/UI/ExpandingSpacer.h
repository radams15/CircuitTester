/**
 * @author Rhys Adams
 * @date 14/01/2022
 */

#ifndef CIRCUITTESTER_EXPANDINGSPACER_H
#define CIRCUITTESTER_EXPANDINGSPACER_H


#include <QWidget>

/** @brief Widget that fills all horizontal space.
 *
 * Used only in toolbar to right-align the hamburger menu.
 */
class ExpandingSpacer : public QWidget{
public:
    /** @brief Initialiser
     *
     * Simply sets the size policy to expand horizontally and do nothing vertically.
     */
    inline ExpandingSpacer() : QWidget(){setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);}
};

#endif //CIRCUITTESTER_EXPANDINGSPACER_H
