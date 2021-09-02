/** @brief
 * @author Rhys Adams
 * @date 05/08/2021
 */

#ifndef CIRCUITTESTER_CPP_SPOILER_H
#define CIRCUITTESTER_CPP_SPOILER_H


#include <QWidget>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QGroupBox>

class SettingsMenu : public QWidget { // https://stackoverflow.com/questions/32476006/how-to-make-an-expandable-collapsable-section-widget-in-qt
Q_OBJECT

private:
    const int animationDuration = 300;
    QParallelAnimationGroup* toggleAnimation;
    QGroupBox* contentArea;

    QGridLayout* mainLayout;

public:
    void startAnimation(bool checked);
    QToolButton* toggleButton;

    SettingsMenu(QWidget* parent = nullptr);

    void setContentLayout(QLayout* contentLayout);

    QVBoxLayout* innerLayout;

    void setInteriorLayout(QLayout* layout);
    void clear();
};


#endif //CIRCUITTESTER_CPP_SPOILER_H
