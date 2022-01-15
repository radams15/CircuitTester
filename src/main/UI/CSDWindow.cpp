#include "CSDWindow.h"

#include <iostream>

CSDWindow::CSDWindow(QWidget *parent) : QMainWindow(parent){
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);

    QFile windowCSS(":/css/custom_window.css");
    windowCSS.open(QFile::ReadOnly);
    setStyleSheet(windowCSS.readAll());
    windowCSS.close();

    titleBar = new CSDTitleBar(this);

    connect(titleBar, &CSDTitleBar::requestClose,
            this, &CSDWindow::close);

    connect(titleBar, &CSDTitleBar::requestMaximize, [this]{ isMaximized() ? showNormal() : showMaximized(); });

    connect(titleBar, &CSDTitleBar::requestMinimize,
            this, &CSDWindow::showMinimized);

    connect(this, &QMainWindow::windowTitleChanged, titleBar,
            &QWidget::setWindowTitle);

    QMainWindow::setMenuWidget(titleBar);
}