//
// Created by rhys on 05/08/2021.
//

#include "SettingsMenu.h"
#include <iostream>

SettingsMenu::SettingsMenu(QWidget *parent) : QWidget(parent){
    toggleAnimation = new QParallelAnimationGroup(parent);
    contentArea = new QScrollArea();
    toggleButton = new QToolButton();
    mainLayout = new QGridLayout();

    innerLayout = new QVBoxLayout();

    //toggleButton->setStyleSheet("QToolButton { border: none; }");
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setArrowType(Qt::LeftArrow);
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);

    contentArea->setStyleSheet("QScrollArea { border: none; }");
    contentArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    contentArea->setMaximumWidth(0);
    contentArea->setMinimumWidth(0);

    toggleAnimation->addAnimation(new QPropertyAnimation(this, "minimumWidth"));
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "maximumWidth"));
    toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumWidth"));

    mainLayout->setVerticalSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(toggleButton, 0, 0, 1, 1, Qt::AlignTop);
    mainLayout->addWidget(contentArea, 0, 1, 1, 1);

    setLayout(mainLayout);

    connect(toggleButton, &QToolButton::clicked, this, &SettingsMenu::startAnimation);

    setContentLayout(innerLayout);
}

void SettingsMenu::startAnimation(bool checked) {
    toggleButton->setArrowType(checked? Qt::RightArrow : Qt::LeftArrow);

    toggleAnimation->setDirection(checked? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    toggleAnimation->start();
}

void SettingsMenu::setContentLayout(QLayout* contentLayout) {
    contentArea->setLayout(contentLayout);

    int collapsedHeight = sizeHint().width() - contentArea->maximumWidth();
    int contentHeight = 200;

    for(int i=0 ; i<toggleAnimation->animationCount()-1 ; i++){
        auto* HelpMenuAnimation = (QPropertyAnimation*) toggleAnimation->animationAt(i);
        HelpMenuAnimation->setDuration(animationDuration);
        HelpMenuAnimation->setStartValue(collapsedHeight);
        HelpMenuAnimation->setEndValue(collapsedHeight+contentHeight);
    }

    auto* contentAnimation = (QPropertyAnimation*) toggleAnimation->animationAt(toggleAnimation->animationCount()-1);
    contentAnimation->setDuration(animationDuration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentHeight);
}

void SettingsMenu::clear() {
    for(int i=0 ; i<innerLayout->count() ; i++){
        innerLayout->itemAt(i)->widget()->setHidden(true);
        innerLayout->removeItem(innerLayout->itemAt(i));
    }
}

void SettingsMenu::setInteriorLayout(QLayout* layout) {
    clear();

    auto* newWidget = new QWidget;
    newWidget->setLayout(layout);

    innerLayout->addWidget(newWidget);
}
