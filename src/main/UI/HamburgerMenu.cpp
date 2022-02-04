//
// Created by rhys on 14/1/22.
//

#include "HamburgerMenu.h"

HamburgerMenu::HamburgerMenu() {
    // Initialise the menu.
    menu = new QMenu();

    // Set the icon to hamburger menu.
    QToolButton::setIcon(QIcon(":/images/application-menu.png"));

    // Set the menu of the toolbutton to a new one.
    setMenu(menu);

    // Make the menu pop down when pressed.
    setPopupMode(InstantPopup);
}