//
// Created by rhys on 07/02/2021.
//

#include "customtitlebar.h"

#define BUTTON_FIX_SIZE(button, size)     button.setMaximumSize(size); button.setMinimumSize(size);

CustomTitlebar::CustomTitlebar(QWidget *parent) : QWidget(parent){
    canMove = false;
    maximised = false;
    buttonStates = WindowAttributes::All;

    minimized = false;

    menu = new QMenu;

    hamburgerMenu.setMenu(menu);
    hamburgerMenu.setPopupMode(QToolButton::InstantPopup);

    QFile titlebarIn(":/css/custom_titlebar.css");
    titlebarIn.open(QFile::ReadOnly);

    titlebarCSS = titlebarIn.readAll().toStdString();

    setStyleSheet(QString::fromStdString(titlebarCSS));

    this->parent = parent;

    windowLabel.setAlignment(Qt::AlignCenter);

    closeButton.setText("X");
    maximiseMutton.setText("+");
    minimiseButton.setText("-");
    hamburgerMenu.setText("|||");

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


void CustomTitlebar::setWindowButtons(WindowAttributes::WindowButtons btns){
    buttonStates = btns;
    closeButton.setVisible(btns & WindowAttributes::Close);
    maximiseMutton.setVisible(btns & WindowAttributes::Maximize);
    minimiseButton.setVisible(btns & WindowAttributes::Minimize);
}

void CustomTitlebar::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void CustomTitlebar::mousePressEvent(QMouseEvent *event){
    if (event->button() & Qt::LeftButton){
        canMove = (event->x() > 5 && event->y() > 5 && event->x() < (parent->width() - 5));

        cursorPos = event->globalPos() - parent->geometry().topLeft();
    }
    QWidget::mousePressEvent(event);
}

void CustomTitlebar::mouseMoveEvent(QMouseEvent *event){
    if (!maximised && canMove && event->buttons() & Qt::LeftButton && !parent->isMaximized()){
        parent->move(event->globalPos() - cursorPos);
    }
    maximised = false;
    QWidget::mouseMoveEvent(event);
}

void CustomTitlebar::mouseDoubleClickEvent(QMouseEvent *event){
    if (buttonStates & WindowAttributes::Maximize && maximiseMutton.isEnabled() && event->buttons() & Qt::LeftButton) {
        maximised = true;
        requestMaximize();
    }
    QWidget::mouseDoubleClickEvent(event);
}