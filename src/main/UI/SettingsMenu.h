//
// Created by rhys on 05/08/2021.
//

#ifndef CIRCUITTESTER_CPP_SPOILER_H
#define CIRCUITTESTER_CPP_SPOILER_H


#include <QWidget>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QGridLayout>
#include <QPropertyAnimation>

class SettingsMenu : public QWidget { // https://stackoverflow.com/questions/32476006/how-to-make-an-expandable-collapsable-section-widget-in-qt
Q_OBJECT

private:
    const int animationDuration = 300;
    QParallelAnimationGroup* toggleAnimation;
    QScrollArea* contentArea;
    QToolButton* toggleButton;
    QGridLayout* mainLayout;

    void startAnimation(bool checked);

public:
    SettingsMenu(QWidget *parent = nullptr);

    void setContentLayout(QLayout* contentLayout);

    QVBoxLayout* innerLayout;

    void setInteriorLayout(QLayout* layout);
    void clear();
};


#endif //CIRCUITTESTER_CPP_SPOILER_H
