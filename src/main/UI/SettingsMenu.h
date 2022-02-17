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

/** @brief Settings menu that dynamically expands and contracts.
 *
 * Adapted from https://stackoverflow.com/questions/32476006/how-to-make-an-expandable-collapsable-section-widget-in-qt, but
 * the orientation has been flipped. And the widget has been restyled.
 *
 * I have also added the changeable interior and the the QGroupBox.
 *
 */
class SettingsMenu : public QWidget {
Q_OBJECT

private:
    /** @brief The time the animation takes to complete in milliseconds.
     *
     */
    const int animationDuration = 300;

    /** @brief The animation to run to show/hide the settings menu.
     *
     */
    QParallelAnimationGroup* toggleAnimation;

    /** @brief The box that contains the innerLayout.
     *
     * This does not change, but its content does.
     *
     */
    QGroupBox* contentArea;

    /** @brief The main layout of the SettingsMenu QWidget.
     *
     */
    QGridLayout* mainLayout;

    /** @brief The layout inside the contentArea that is changed by different components.
     *
     */
    QVBoxLayout* innerLayout;

    /** @brief Sets the layout of contentArea, configures toggleAnimation to show/hide the layout.
     *
     * @param contentLayout
     */
    void setContentLayout(QLayout* contentLayout);

    /** @brief Deletes the widgets inside innerLayout.
     *
     */
    void clear();

    /** Start the animation to show/hide the settings menu.
     *
     * @param checked Bool to decide which direction to go.
     * Checked=true causes the settings to open.
     * Checked=false causes the settings to close.
     */
    void startAnimation(bool checked);

public:
    /** @brief Button widget that is used to open/close the settings window.
     *
     */
    QToolButton* toggleButton;

    /** @brief Initialiser.
     *
     */
    SettingsMenu(std::string name);

    /** @brief Sets the layout of the widget, used to set different settings panels for
     * each component.
     *
     * @param layout The layout pointer to set.
     */
    void setInteriorLayout(QLayout* layout);
};


#endif //CIRCUITTESTER_CPP_SPOILER_H
