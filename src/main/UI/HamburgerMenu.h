/**
 * @author Rhys Adams
 * @date 14/01/2022
 */

#ifndef CIRCUITTESTER_HAMBURGERMENU_H
#define CIRCUITTESTER_HAMBURGERMENU_H


#include <QToolButton>
#include <QMenu>

/** @brief Simple hamburger menu.
 *
 * Simply derives from a toolbutton, is used to simply create a hamburger menu.
 */
class HamburgerMenu : public QToolButton{
private:

public:
    /** @brief QMenu that stores menu data for the toolbutton.
     *
     * Can be set externally to public.
     */
    QMenu* menu;

    /** @brief Initialiser.
     *
     * Creates menu, sets icon.
     */
    HamburgerMenu();
};


#endif //CIRCUITTESTER_HAMBURGERMENU_H
