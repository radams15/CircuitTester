#ifndef CSDWINDOW_H
#define CSDWINDOW_H

#include <QMenu>
#include <QRect>
#include <QPoint>
#include <QObject>
#include <QWidget>
#include <QCursor>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMetaMethod>
#include <QMetaObject>
#include <QDockWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QVBoxLayout>

#include "CSDTitlebar.h"

class CSDWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit CSDWindow(QWidget *parent = nullptr);

protected:
    CSDTitleBar *titleBar;
};

#endif // CSDWINDOW_H
