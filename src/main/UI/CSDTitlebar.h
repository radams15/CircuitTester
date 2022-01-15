//
// Created by rhys on 07/02/2021.
//

#ifndef CSD_CSDTITLEBAR_H
#define CSD_CSDTITLEBAR_H

#include <QSize>
#include <QEvent>
#include <QLabel>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPushButton>
#include <QStyleOption>
#include <QMenu>
#include <QToolButton>

#define BUTTON_SIZE QSize(24, 24)

namespace WindowAttributes {
    enum WindowButton {
        Minimize = 0x01,
        Maximize = 0x02,
        Close    = 0x04,
        All      = Minimize | Maximize | Close
    };

    Q_DECLARE_FLAGS(WindowButtons, WindowButton)
    Q_DECLARE_OPERATORS_FOR_FLAGS(WindowButtons)
}



class CSDTitleBar : public QWidget{
    Q_OBJECT
    Q_CLASSINFO("CSDTitleBar", "QTitleBar")

public:
    explicit CSDTitleBar(QWidget *parent = nullptr);

    QMenu* menu;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    bool canMove;

    QPoint cursorPos;

    QWidget *parent;

    WindowAttributes::WindowButtons buttonStates;

    QLabel windowLabel;
    QHBoxLayout windowLayout;

    QPushButton minimiseButton;
    QPushButton maximiseMutton;
    QPushButton closeButton;
    QToolButton hamburgerMenu;

signals:
    void requestClose();
    void requestMaximize();
    void requestMinimize();

};


#endif //CSD_CSDTITLEBAR_H
