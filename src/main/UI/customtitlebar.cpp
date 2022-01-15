//
// Created by rhys on 07/02/2021.
//

#include "customtitlebar.h"

CustomTitlebar::CustomTitlebar(QWidget *parent) : QWidget(parent), FRAME_BUTTON_SIZE(24, 24){
    canMove = false;
    maximizing = false;
    m_frameButtons = QCustomAttrs::All;

    menu = new QMenu;

    btn_menu.setMenu(menu);
    btn_menu.setPopupMode(QToolButton::InstantPopup);

    QFile titlebarIn(":/css/custom_titlebar.css");
    titlebarIn.open(QFile::ReadOnly);

    custom_titlebar_css = titlebarIn.readAll().toStdString();

    setStyleSheet(QString::fromStdString(custom_titlebar_css));

    this->m_parentWindow = parent;

    this->lbl_windowTitle.setText("CircuitTester");
    this->lbl_windowTitle.setAlignment(Qt::AlignCenter);

    this->btn_close.setText("X");
    this->btn_maximize.setText("+");
    this->btn_minimize.setText("-");
    this->btn_menu.setText("|||");

    this->btn_close.setMaximumSize(FRAME_BUTTON_SIZE);
    this->btn_close.setMinimumSize(FRAME_BUTTON_SIZE);
    this->btn_maximize.setMaximumSize(FRAME_BUTTON_SIZE);
    this->btn_maximize.setMinimumSize(FRAME_BUTTON_SIZE);
    this->btn_minimize.setMaximumSize(FRAME_BUTTON_SIZE);
    this->btn_minimize.setMinimumSize(FRAME_BUTTON_SIZE);
    this->btn_menu.setMaximumSize(FRAME_BUTTON_SIZE);
    this->btn_menu.setMinimumSize(FRAME_BUTTON_SIZE);

    this->m_layout.addWidget(&this->btn_menu);
    this->m_layout.addWidget(&this->lbl_windowTitle, 1);
    this->m_layout.addWidget(&this->btn_minimize);
    this->m_layout.addWidget(&this->btn_maximize);
    this->m_layout.addWidget(&this->btn_close);
    this->m_layout.setContentsMargins(0, 0, 0, 4);
    this->m_layout.setSpacing(0);

    this->setLayout(&this->m_layout);

    connect(&this->btn_close, &QPushButton::clicked, [this]{ this->requestClose(); });
    connect(&this->btn_minimize, &QPushButton::clicked, [this]{ this->requestMinimize(); });
    connect(&this->btn_maximize, &QPushButton::clicked, [this]{ this->requestMaximize(); });

    //connect(&this->btn_menu, &QPushButton::clicked, [this]{ this->requestMaximize(); });

    connect(this, &QWidget::windowTitleChanged, &this->lbl_windowTitle, &QLabel::setText);
}


void CustomTitlebar::setWindowButtons(QCustomAttrs::WindowButtons btns){
    this->m_frameButtons = btns;
    this->btn_close.setVisible(btns & QCustomAttrs::Close);
    this->btn_maximize.setVisible(btns & QCustomAttrs::Maximize);
    this->btn_minimize.setVisible(btns & QCustomAttrs::Minimize);
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
        this->canMove = (event->x() > 5 && event->y() > 5 && event->x() < (this->m_parentWindow->width() - 5));
        this->m_pCursor = event->globalPos() - this->m_parentWindow->geometry().topLeft();
    }
    QWidget::mousePressEvent(event);
}

void CustomTitlebar::mouseMoveEvent(QMouseEvent *event){
    if (!this->maximizing && canMove && event->buttons() & Qt::LeftButton
        && !this->m_parentWindow->isMaximized()) this->m_parentWindow->move(event->globalPos() - m_pCursor);
    this->maximizing = false;
    QWidget::mouseMoveEvent(event);
}

void CustomTitlebar::mouseDoubleClickEvent(QMouseEvent *event){
    if (m_frameButtons & QCustomAttrs::Maximize && btn_maximize.isEnabled()
        && event->buttons() & Qt::LeftButton) {
        this->maximizing = true;
        requestMaximize();
    }
    QWidget::mouseDoubleClickEvent(event);
}