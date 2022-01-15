//
// Created by rhys on 07/02/2021.
//

#ifndef CSD_CUSTOMTITLEBAR_H
#define CSD_CUSTOMTITLEBAR_H

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

struct ButtonStates{
    bool maximise;
    bool minimise;
};

class CustomTitlebar : public QWidget{
    Q_OBJECT
    Q_PROPERTY(WindowAttributes::WindowButtons windowButtons READ windowButtons WRITE setWindowButtons)
    Q_CLASSINFO("CustomTitlebar", "QTitleBar")

public:
    explicit CustomTitlebar(QWidget *parent = nullptr);

    void setWindowButtons(WindowAttributes::WindowButtons btns);
    inline WindowAttributes::WindowButtons windowButtons() const { return this->buttonStates; }

    QMenu* menu;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    bool canMove;
    bool maximised;
    bool minimized;

    QPoint cursorPos;

    QWidget *parent;

    WindowAttributes::WindowButtons buttonStates;

    QLabel windowLabel;
    QHBoxLayout windowLayout;

    std::string titlebarCSS;

    QToolButton minimiseButton;
    QToolButton maximiseMutton;
    QToolButton closeButton;
    QToolButton hamburgerMenu;

signals:
    void requestClose();
    void requestMaximize();
    void requestMinimize();

};


#endif //CSD_CUSTOMTITLEBAR_H
