//
// Created by rhys on 1/14/22.
//

#ifndef CIRCUITTESTER_HAMBURGERMENU_H
#define CIRCUITTESTER_HAMBURGERMENU_H


#include <QtGui/QToolButton>
#include <QtGui/QMenu>

class HamburgerMenu : public QToolButton{
private:

public:
    QMenu* menu;

    HamburgerMenu();
};


#endif //CIRCUITTESTER_HAMBURGERMENU_H
