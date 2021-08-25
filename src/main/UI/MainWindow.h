/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>
#include <vector>

#include <QMainWindow>
#include <QAbstractButton>
#include <QComboBox>
#include <QFontComboBox>
#include <QToolBox>
#include <QToolButton>
#include <QGraphicsView>

#include "UIComponent.h"
#include "SceneItem.h"
#include "Scene.h"
#include "SettingsMenu.h"

#define CANVAS_SIZE 1000, 1000
#define WINDOW_SIZE 1200, 700

/** @brief The main window class that is the GUI for the user on startup.
 *
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /** @brief Initialiser for the main window object.
     *
     */
    MainWindow();

    /** @brief Called from the Scene when any SceneItem is right clicked.
     * Places the settings widget from the SceneItem into the SettingsMenu.
     *
     * @param item The item that has been right clicked.
     */
    void itemRightClicked(UIComponent* item);

private slots:
    /** @brief Called when any component button is clicked.
     *
     * @param button The button that was clicked.
     */
    void buttonGroupClicked(QAbstractButton* button);

    /** @brief Called when the delete button is pressed. Deletes all items that are selected.
     *
     */
    void deleteItem();

    /** @brief Called when either the drag button or the line buttons are pressed.
     *
     */
    void pointerGroupClicked();

    /** @brief Called when any item is inserted onto the Scene. This unchecks the tool button
     * of that item.
     *
     * @param item The item that has been inserted.
     */
    void itemInserted(UIComponent* item);

    /** @brief Called when the scene scale QComboBox is changed.
     * This changes the scale of the Scene.
     *
     * @param scale
     */
    void sceneScaleChanged(const QString &scale);

    /** @brief Called when the about button is clicked from the toolbar.
     *
     */
    void about();

    /** @brief Called when the save button is clicked from the toolbar.
     *
     */
    void saveScene();

    /** @brief Called when the open button is clicked from the toolbar.
     *
     */
    void openScene();

    /** @brief Called when the import button is clicked from the toolbar.
     *
     */
    void importScene();

    /** @brief Called when the export button is clicked from the toolbar.
     *
     */
    void exportScene();

    /** @brief Called when the run button is clicked from the toolbar.
     *
    */
    void runSimulation();

private:
    /** @brief The name of the circuit that is open.
     *
     * This is empty before a circuit is opened or saved.
     *
     */
    std::string currentOpenedCircuit;

    /** @brief Creates the toolbox of components.
     *
     */
    void createToolBox();

    /** @brief Creates all actions for the toolbar and the menubar.
     *
     */
    void createActions();

    /** @brief Creates the menubar for the actions.
    *
    */
    void createMenus();

    /** @brief Creates the toolbar for the actions.
    *
    */
    void createToolbars();

    /** @brief Creates a tool button for a component.
     *
     * @tparam T The class of component to create a button for.
     * @param text The text for the label under the button.
     * @return A QWidget pointer to the newly created widget.
     */
    template<class T> QWidget* createCellWidget(const QString &text);

    /** @brief The scene where the SceneItems are placed.
     *
     */
    Scene* scene;

    /** @brief The view that shows the Scene.
     *
     */
    QGraphicsView* view;

    /** @brief The collapsable settings menu for components.
     *
     */
    SettingsMenu* settingsMenu;

    /** @brief An action that exits the application.
     *
     */
    QAction* exitAction;

    /** @brief An action that deletes the selected component.
     *
     */
    QAction* deleteAction;

    /** @brief An action that runs the simulation.
     *
     */
    QAction* runAction;

    /** @brief An action that shows the about popup.
     *
     */
    QAction* aboutAction;

    /** @brief An action that saves the scene.
     *
     */
    QAction* saveAction;

    /** @brief An action that opens a save file.
     *
     */
    QAction* openAction;

    /** @brief An action that exports the scene.
 *
 */
    QAction* exportAction;

    /** @brief An action that imports a save file.
     *
     */
    QAction* importAction;

    /** @brief The file menu on the menubar.
     *
     */
    QMenu* fileMenu;

    /** @brief The item menu on the menubar.
     *
     */
    QMenu* itemMenu;

    /** @brief The about menu on the menubar.
     *
     */
    QMenu* aboutMenu;

    /** @brief The simulation menu on the menubar.
     *
     */
    QMenu* simMenu;

    /** @brief The toolbar that allows you to select either movement or line mode.
     *
     */
    QToolBar* pointerToolbar;

    /** @brief The QComboBox that allows you to select the scene scale.
     *
     */
    QComboBox* sceneScaleCombo;

    /** @brief The toolbox that contains the component buttons.
     *
     */
    QToolBox* toolBox;

    /** @brief The button group that holds the multiple components.
     *
     */
    QButtonGroup* buttonGroup;

    /** @brief The button group that holds buttons for movement or line mode.
     *
     */
    QButtonGroup* pointerTypeGroup;
};


#endif // MAINWINDOW_H
