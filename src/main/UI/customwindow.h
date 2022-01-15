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

class CustomWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit CustomWindow(QWidget *parent = nullptr);
    ~CustomWindow() override;

    QMenu * createPopupMenu() override;

    void setMenuBar(QMenuBar *menuBar);
    QMenuBar * menuBar() const;

    void setMenuWidget(QWidget *widget);
    QWidget * menuWidget() const;

    inline CustomTitlebar& titleBar() const { return *this->m_titleBar; }

protected:
    const int RESIZE_LIMIT;

    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

    void customMouseMoveEvent(QMouseEvent* event);

    CustomTitlebar *m_titleBar;

private:
    QWidget *m_titleBarW;
    QWidget *m_menuWidget;
    QMenuBar *m_menuBar;

    QToolBar *m_leftBorder;
    QToolBar *m_rightBorder;
    QToolBar *m_bottomBorder;

    Qt::Edges m_lock;
    QPoint m_posCursor;

    std::string custom_window_css;
    std::string custom_toolbar_css;

    QToolBar * generateBorder(Qt::ToolBarArea area, Qt::Orientation orientation);
};

#endif // CUSTOMWINDOW_H
