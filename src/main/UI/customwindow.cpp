#include "customwindow.h"

#include <iostream>

CSDWindow::CSDWindow(QWidget *parent) : QMainWindow(parent){
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);

    QFile windowIn(":/css/custom_window.css");
    windowIn.open(QFile::ReadOnly);
    std::string windowCss = windowIn.readAll().toStdString();

    setStyleSheet(QString::fromStdString(windowCss));

    titleBar = new CustomTitlebar(this);

    connect(titleBar, &CustomTitlebar::requestClose,
            this, &CSDWindow::close);

    connect(titleBar, &CustomTitlebar::requestMaximize, [this]{ isMaximized() ? showNormal() : showMaximized(); });

    connect(titleBar, &CustomTitlebar::requestMinimize,
            this, &CSDWindow::showMinimized);

    connect(this, &QMainWindow::windowTitleChanged, titleBar,
            &QWidget::setWindowTitle);

    QMainWindow::setMenuWidget(titleBar);
}