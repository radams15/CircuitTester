//
// Created by rhys on 1/14/22.
//

#include "HamburgerMenu.h"

HamburgerMenu::HamburgerMenu() {
    menu = new QMenu();

    QToolButton::setIcon(QIcon(":/images/application-menu.png"));

    setMenu(menu);
    setPopupMode(InstantPopup);
}