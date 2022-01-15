#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

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

#include "customtitlebar.h"

class CSDWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit CSDWindow(QWidget *parent = nullptr);

protected:
    CustomTitlebar *titleBar;
};

#endif // CUSTOMWINDOW_H
