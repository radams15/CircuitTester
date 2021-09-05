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
    // Set width to 0 initially so it starts minimised.
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
    // Change arrow depending on direction. E.g. right arrow to close if open, left arrow to open if closed.
    toggleButton->setArrowType(checked? Qt::RightArrow : Qt::LeftArrow);

    // Set the animation to move forward or backward, e.g. to open move the animation forward,
    // to close move the animation backward.
    toggleAnimation->setDirection(checked? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    // Begin the animation.
    toggleAnimation->start();
}

void SettingsMenu::setContentLayout(QLayout* contentLayout) {
    contentArea->setLayout(contentLayout);

    // Width of the content area when it should be collapsed.
    int collapsedWidth = sizeHint().width() - contentArea->maximumWidth();
    int contentWidth = 200;

    // Run through each animation except the last to set the properties.
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
        // Hide the widget so it disappears.
        innerLayout->itemAt(i)->widget()->setHidden(true);

        // Remove the widget.
        innerLayout->removeItem(innerLayout->itemAt(i));
    }
}

void SettingsMenu::setInteriorLayout(QLayout* layout) {
    // Check layout is not a null pointer.
    if(layout == nullptr){
        return;
    }

    // Remove any widgets there already.
    clear();

    // Create a widget, set the layout to the passed layout.
    auto* newWidget = new QWidget;
    newWidget->setLayout(layout);

    // Add the widget to the inner layout.
    innerLayout->addWidget(newWidget);
}
