/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>
#include <vector>

#include <QtGui/QMainWindow>
#include <QtGui/QAbstractButton>
#include "UIComponent.h"
#include "SceneItem.h"
#include "Scene.h"
#include "SettingsMenu.h"
#include "HamburgerMenu.h"

#define CANVAS_SIZE 1000, 1000
#define WINDOW_SIZE 1200, 700

#define HAMBURGER_MENU true

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

    /** @brief Called from the Scene when any SceneItem is double clicked.
     * Places the settings widget from the SceneItem into the SettingsMenu.
     *
     * @param item The item that has been double clicked.
     */
    void itemDoubleClicked(UIComponent* item);

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
	void moveActionClicked();
	void lineActionClicked();

    /** @brief Called when any item is inserted onto the Scene. This unchecks the tool button
     * of that item.
     *
     * @param item The item that has been inserted.
     */
    void itemInserted(UIComponent* item);

    /** @brief Called when the about button is clicked from the toolbar.
     *
     */
    void about();

    /** @brief Called when the tutorial button is clicked from the toolbar.
     *
     */
    void tutorial();

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

    /** @brief Called when the open save directory button is clicked from the toolbar.
     *
     */
    static void openSaveDir();

    /** @brief Called when the run button is clicked from the toolbar.
     *
    */
    void runSimulation();

    int getMode();

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
    void createMenubar();

    /** @brief Creates the toolbar for the actions.
    *
    */
    void createToolbar();

    /** @brief Creates a tool button for a component.
     *
     * @tparam T The class of component to create a button for.
     * @param text The text for the label under the button.
     * @return A QWidget pointer to the newly created widget.
     */
    template<class T> QWidget* createCellWidget(std::string text);

    /** @brief The scene where the SceneItems are placed.
     *
     */
    Scene* scene;

    /** @brief The view that shows the Scene.
     *
     */
    QGraphicsView* view;

#if HAMBURGER_MENU
    HamburgerMenu* mainMenu;
#else
    QWidget* mainMenu;
#endif

    QMenuBar* mainMenuBar;

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

    /** @brief An action that shows the about popup.
     *
     */
    QAction* aboutAction;

    /** @brief An action that shows the tutorial popup.
     *
     */
    QAction* tutorialAction;

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

    /** @brief An action that opens the save directory.
     *
     */
    QAction* saveDirAction;

    /** @brief An moves components.
     *
     */
    QAction* moveAction;

    /** @brief An action that draws lines between components.
     *
     */
    QAction* lineAction;

    /** @brief Sets globally whether or not the simulation is running.
     *
     */
    QAction* runningAction;

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

    /** @brief The toolbar that allows you to select either movement or line mode.
     *
     */
    QToolBar* pointerToolbar;

    /** @brief The toolbox that contains the component buttons.
     *
     */
    QTabWidget* componentTabs;

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
