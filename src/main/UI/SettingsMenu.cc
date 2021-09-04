//
// Created by rhys on 05/08/2021.
//

#include "SettingsMenu.h"
#include <iostream>

SettingsMenu::SettingsMenu() : QWidget(nullptr){
    // Create an animation group
    toggleAnimation = new QParallelAnimationGroup(nullptr);
    // Create the container with the title of settings.
    contentArea = new QGroupBox("Settings");
    toggleButton = new QToolButton();
    mainLayout = new QGridLayout();

    innerLayout = new QVBoxLayout();

    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setArrowType(Qt::LeftArrow);
    toggleButton->setCheckable(true);
    // Default to closed.
    toggleButton->setChecked(false);

    // Fixed horizontal size, expands vertically as much as possible.
    contentArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    // Set width to 0.
    contentArea->setMaximumWidth(0);
    contentArea->setMinimumWidth(0);

    toggleAnimation->addAnimation(new QPropertyAnimation(this, "minimumWidth"));
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "maximumWidth"));
    toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumWidth"));

    mainLayout->setVerticalSpacing(0);
    // No margins.
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Add the button at coordinates 0, 0.
    mainLayout->addWidget(toggleButton, 0, 0, 1, 1, Qt::AlignTop);
    // Add the changeable widget at coordinates 0, 1, i.e. to the right of the button.
    mainLayout->addWidget(contentArea, 0, 1, 1, 1);

    setLayout(mainLayout);

    // Start animation when toggle button is clicked.
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

    int collapsedWidth = sizeHint().width() - contentArea->maximumWidth();
    int contentWidth = 200;

    for(int i=0 ; i<toggleAnimation->animationCount()-1 ; i++){
        auto* HelpMenuAnimation = (QPropertyAnimation*) toggleAnimation->animationAt(i);
        HelpMenuAnimation->setDuration(animationDuration);
        HelpMenuAnimation->setStartValue(collapsedWidth);
        HelpMenuAnimation->setEndValue(collapsedWidth + contentWidth);
    }

    auto* contentAnimation = (QPropertyAnimation*) toggleAnimation->animationAt(toggleAnimation->animationCount()-1);
    contentAnimation->setDuration(animationDuration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentWidth);
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
