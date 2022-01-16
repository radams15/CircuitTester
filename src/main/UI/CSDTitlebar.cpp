//
// Created by rhys on 07/02/2021.
//

#include <iostream>
#include "CSDTitlebar.h"

#define BUTTON_FIX_SIZE(button, size)     button.setMaximumSize(size); button.setMinimumSize(size);

CSDTitleBar::CSDTitleBar(QWidget *parent) : QWidget(parent){
    canMove = false;
    buttonStates = WindowAttributes::All;

    menu = new QMenu;

    hamburgerMenu.setMenu(menu);
    hamburgerMenu.setPopupMode(QToolButton::InstantPopup);

    QFile titlebarCSS(":/css/custom_titlebar.css");
    titlebarCSS.open(QFile::ReadOnly);
    QString css = titlebarCSS.readAll();
    setStyleSheet(css);
    //menu->setStyleSheet(css);
    titlebarCSS.close();

    this->parent = parent;

    windowLabel.setAlignment(Qt::AlignCenter);

    closeButton.setIcon(QIcon(":/images/window-close-symbolic.png"));
    maximiseMutton.setIcon(QIcon(":/images/window-maximize-symbolic.png"));
    minimiseButton.setIcon(QIcon(":/images/window-minimize-symbolic.png"));
    hamburgerMenu.setIcon(QIcon(":/images/application-menu.png"));

    BUTTON_FIX_SIZE(closeButton, BUTTON_SIZE);
    BUTTON_FIX_SIZE(maximiseMutton, BUTTON_SIZE);
    BUTTON_FIX_SIZE(minimiseButton, BUTTON_SIZE);
    BUTTON_FIX_SIZE(hamburgerMenu, BUTTON_SIZE);

    windowLayout.addWidget(&windowLabel, 1);
    windowLayout.addWidget(&hamburgerMenu);
    windowLayout.addWidget(&minimiseButton);
    windowLayout.addWidget(&maximiseMutton);
    windowLayout.addWidget(&closeButton);
    windowLayout.setSpacing(0);

    setLayout(&windowLayout);

    connect(&closeButton, &QPushButton::clicked, [this]{ requestClose(); });
    connect(&minimiseButton, &QPushButton::clicked, [this]{ requestMinimize(); });
    connect(&maximiseMutton, &QPushButton::clicked, [this]{ requestMaximize(); });

    connect(this, &QWidget::windowTitleChanged, &windowLabel, &QLabel::setText);
}

void CSDTitleBar::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void CSDTitleBar::mousePressEvent(QMouseEvent *event){
    if (event->button() & Qt::LeftButton){
        canMove = (event->x() > 5 && event->y() > 5 && event->x() < (parent->width() - 5));

        cursorPos = event->globalPos() - parent->geometry().topLeft();
    }
    QWidget::mousePressEvent(event);
}

void CSDTitleBar::mouseMoveEvent(QMouseEvent *event){
    if (canMove && event->buttons() & Qt::LeftButton && !parent->isMaximized()){
        parent->move(event->globalPos() - cursorPos);
    }
    QWidget::mouseMoveEvent(event);
}

void CSDTitleBar::mouseDoubleClickEvent(QMouseEvent *event){
    if (buttonStates & WindowAttributes::Maximize && maximiseMutton.isEnabled() && event->buttons() & Qt::LeftButton) {
        requestMaximize();
    }
    QWidget::mouseDoubleClickEvent(event);
}