#include <vector>
#include "Line.h"
#include "SceneItem.h"
#include "Scene.h"
#include "SceneText.h"
#include "MainWindow.h"

#include "Components/Resistor.h"
#include "Components/Battery.h"
#include "Components/Wire.h"
#include "Components/Switch.h"
#include "Components/Voltmeter.h"
#include "Components/Ammeter.h"

#include "../Saves/CircuitSaver.h"
#include "AnalysisMapper.h"

#include "../Saves/FileUtils.h"
#include "ExpandingSpacer.h"
#include "HamburgerMenu.h"

#include <QtWidgets>
#include <QInputDialog>
#include <iostream>
#include <sstream>

#ifdef WIN32
#include <shellapi.h>
#endif

/**
 * \def SHOW_ICON(toolbar, action)
 * Gets the toolbutton for the action, sets the toolbutton to show the text and icon.
*/
#define SHOW_ICON(toolbar, action) ((QToolButton*)toolbar->widgetForAction(action))->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

MainWindow::MainWindow() {
    createActions();
    createComponentView();
    createMenubar();

    // Set window icon to the connector image.
    setWindowIcon(QIcon(":/images/linepointer.png"));

    // Create the scene, set it to the specified size.
    scene = new Scene(this);
    scene->setSize(CANVAS_SIZE);

    // Resize the window
    resize(WINDOW_SIZE);

    // Call itemInserted when an item is inserted into the scene.
    connect(scene, &Scene::itemInserted,this, &MainWindow::itemInserted);

    // Call itemDoubleClicked when the scene triggers the event.
    connect(scene, &Scene::itemDoubleClicked,this, &MainWindow::itemDoubleClicked);

    createToolbar();

    // Create the main layout, add the graphics view.
    auto* layout = new QHBoxLayout;
    auto* compLayout = new QVBoxLayout;
    compLayout->addWidget(componentTabs);

    settingsMenu2 = new SettingsMenu("Components");
    layout->addWidget(settingsMenu2);
    settingsMenu2->setInteriorLayout(compLayout);
    settingsMenu2->toggleButton->click();

    view = new QGraphicsView(scene);
    layout->addWidget(view);

    // Create and add the settings menu.
    settingsMenu = new SettingsMenu("Settings");
    layout->addWidget(settingsMenu);

    // Create a widget to hold the layout.
    auto* mainWidget = new QWidget;
    mainWidget->setLayout(layout);

    // Set the mainWidget to the centre of the window.
    setCentralWidget(mainWidget);
    setWindowTitle(tr("Circuit Simulator"));

    // Makes the toolbar on mac look the same colour as the titlebar - just aesthetic.
    setUnifiedTitleAndToolBarOnMac(true);

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::runSimulation);
    timer->start(1000); //time specified in ms
}


void MainWindow::buttonGroupClicked(QAbstractButton *button) {
    const QList<QAbstractButton *> buttons = buttonGroup->buttons();

    for (QAbstractButton* myButton : buttons) {
        // Uncheck all the buttons that are not the clicked one.
        if (myButton != button){
            button->setChecked(false);
        }
    }
    const int id = buttonGroup->id(button);

    // Set the scene item to a new instance of the selected button.
    switch(id){
        case UI_RESISTOR:
            scene->setItem(new Resistor);
            break;
        case UI_BATTERY:
            scene->setItem(new Battery);
            break;
        case UI_WIRE:
            scene->setItem(new Wire);
            break;
        case UI_SWITCH:
            scene->setItem(new Switch);
            break;
        case UI_VOLTMETER:
            scene->setItem(new Voltmeter);
            break;
        case UI_AMMETER:
            scene->setItem(new Ammeter);
            break;
        default:
            return;
    }

    scene->setMode(Scene::INSERT_ITEM);
}



void MainWindow::deleteItem() {
    // First delete all the lines.
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : selectedItems) {
        if (item->type() == Line::Type) {
            scene->removeItem(item);
            auto *arrow = (Line*) item;
            arrow->startItem()->removeLine(arrow);
            arrow->endItem()->removeLine(arrow);
            delete item;
        }
    }

    // Then delete all items.
    selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : selectedItems) {
        // Delete all items that are selected
         if (item->type() == SceneItem::Type) {
             // If it's a SceneItem delete all the lines on it also.
             ((SceneItem*) item)->removeLines();
         }
         scene->removeItem(item);
         delete item;
     }
}



void MainWindow::pointerGroupClicked(QAction* action) {
    // Selected either line or move mode, set the mode.

    if(action == moveAction){
        moveAction->setChecked(true);
        lineAction->setChecked(false);
    }else{
        moveAction->setChecked(false);
        lineAction->setChecked(true);
    }

    scene->setMode(Scene::Mode(getMode()));
}


void MainWindow::itemInserted(UIComponent* c) {
    // Check the move mode button.
    moveAction->setChecked(true);
    lineAction->setChecked(false);

    // Set the mode to move mode.
    scene->setMode(Scene::Mode((int) Scene::MOVE));

    // Uncheck the button of that component.
    buttonGroup->button(c->getId())->setChecked(false);
}

void MainWindow::about() {
    // Start simple about box
    QMessageBox::about(this, "About Circuit Simulator",
                       "This is a circuit simulation program written by Rhys Adams (2021-22)");
}

void MainWindow::tutorial() {
    // Start simple about box
    QMessageBox::about(this, "Tutorial",
                       "Welcome to Rhys' circuit simulation program.\n\nHere is a brief tutorial to understand how the program works.\n\nTo place down a component, first click on the component button, then click on the location\non the canvas where you want the component to go.\n\nYou can move components by dragging the black parts.\n\nAt the top left, you can either select movement mode (the mouse pointer button) to drag components,\nor the connector button (the black line to the left of the mouse pointer) to connect components\nwith lines.\n\nDouble-clicking any component will open up the settings menu on the right, which allows you to\nchange anything about any of the Components, such as voltage, resistance or whether they are on or off.\n\nYou can reach this info again at the help->Tutorial menu.");
}


void MainWindow::createComponentView() {
    // Create the button group.
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);

    // When the button group is clicked, call buttonGroupClicked.
    connect(buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
            this, &MainWindow::buttonGroupClicked); // https://doc.qt.io/archives/qt-5.6/qbuttongroup.html#buttonClicked

    // Create new grid basicLayout, add the widgets for each component type to a 2*n grid.
    auto* basicLayout = new QGridLayout;
    basicLayout->addWidget(createCellWidget<Resistor>("Resistor"), 0, 0);
    basicLayout->addWidget(createCellWidget<Battery>("Battery"), 0, 1);
    basicLayout->addWidget(createCellWidget<Wire>("Wire"), 1, 0);
    basicLayout->addWidget(createCellWidget<Switch>("Switch"), 1, 1);

    auto* measurementLayout = new QGridLayout;
    measurementLayout->addWidget(createCellWidget<Ammeter>("Ammeter"), 0, 0);
    measurementLayout->addWidget(createCellWidget<Voltmeter>("Voltmeter"), 0, 1);

    // Don't stretch completely, just 3 pixels stretch per row.
    basicLayout->setRowStretch(3, 10);
    measurementLayout->setRowStretch(3, 10);

    // Create a widget to hold the basicLayout, add the widget.
    auto* basicWidget = new QWidget;
    basicWidget->setLayout(basicLayout);

    // Create a widget to hold the measurementLayout, add the widget.
    auto* measurementWidget = new QWidget;
    measurementWidget->setLayout(measurementLayout);

    // Create a componentTabs to hold the components.
    componentTabs = new QTabWidget;
    componentTabs->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    componentTabs->setMinimumWidth(basicWidget->sizeHint().width()+20);
    componentTabs->setTabPosition(QTabWidget::West);
    componentTabs->addTab(basicWidget, "Basic Components");
    componentTabs->addTab(measurementWidget, "Measurement Components");
}

void incrementCombo(QComboBox* cb){
    int next = cb->currentIndex()+1;

    if(next < cb->count()) {
        cb->setCurrentIndex(next);
    }
}

void decrementCombo(QComboBox* cb){
    int next = cb->currentIndex()-1;

    if(next >= 0) {
        cb->setCurrentIndex(next);
    }
}

void MainWindow::createActions() {
    // Create many actions which can be placed in menubars or toolbars.

    // The ambersand is placed before the letter that is used for alt-navigation.
    deleteAction = new QAction("", this);
    deleteAction->setText(tr("Delete"));
    deleteAction->setIcon(QIcon(":/images/edit-delete.png"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    deleteAction->setShortcut(Qt::Key_Delete);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);

    aboutAction = new QAction("", this);
    aboutAction->setText(tr("About"));
    aboutAction->setIcon(QIcon(":/images/about.png"));
    aboutAction->setStatusTip(tr("About the program"));
    aboutAction->setShortcut(Qt::CTRL + Qt::Key_B);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    exitAction = new QAction("", this);
    exitAction->setText(tr("Exit"));
    exitAction->setIcon(QIcon(":/images/close.png"));
    exitAction->setStatusTip(tr("Exit the program"));
    exitAction->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(exitAction, &QAction::triggered, this, []{ exit(0);});

    tutorialAction = new QAction("", this);
    tutorialAction->setText(tr("T&utorial"));
    tutorialAction->setIcon(QIcon(":/images/dialog-question.png"));
    tutorialAction->setStatusTip(tr("How to use this program"));
    tutorialAction->setShortcut( Qt::Key_F2);
    connect(tutorialAction, &QAction::triggered, this, &MainWindow::tutorial);

    saveAction = new QAction("", this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setText(tr("&Save"));
    saveAction->setIcon(QIcon(":/images/document-save.png"));
    saveAction->setStatusTip(tr("Save the current circuit."));
    saveAction->setShortcut( Qt::CTRL + Qt::Key_S);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveScene);

    openAction = new QAction("", this);
    openAction->setText(tr("&Open"));
    openAction->setIcon(QIcon(":/images/document-open.png"));
    openAction->setStatusTip(tr("Open a saved circuit."));
    openAction->setShortcut( Qt::CTRL + Qt::Key_O);
    connect(openAction, &QAction::triggered, this, &MainWindow::openScene);

    importAction = new QAction("", this);
    importAction->setText(tr("&Import"));
    importAction->setIcon(QIcon(":/images/document-open.png"));
    importAction->setStatusTip(tr("Import a saved circuit."));
    importAction->setShortcut(Qt::CTRL + Qt::Key_I);
    connect(importAction, &QAction::triggered, this, &MainWindow::importScene);

    exportAction = new QAction("", this);
    exportAction->setText(tr("&Export"));
    exportAction->setIcon(QIcon(":/images/document-save.png"));
    exportAction->setStatusTip(tr("Export a saved circuit."));
    exportAction->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportScene);

    saveDirAction = new QAction("", this);
    saveDirAction->setText(tr("Sa&ve Directory"));
    saveDirAction->setIcon(QIcon(":/images/document-open.png"));
    saveDirAction->setStatusTip(tr("Open the save directory in the file manager."));
    saveDirAction->setShortcut(Qt::CTRL + Qt::Key_A);
    connect(saveDirAction, &QAction::triggered, this, &MainWindow::openSaveDir);

    moveAction = new QAction("", this);
    moveAction->setText(tr("&Move"));
    moveAction->setStatusTip(tr("Move a component."));
    moveAction->setIcon(QIcon(":/images/pointer.png"));
    moveAction->setCheckable(true);
    moveAction->setChecked(true);
    moveAction->setShortcut(Qt::Key_M);
    connect(moveAction, &QAction::triggered, this, [this]{ pointerGroupClicked(moveAction); });

    lineAction = new QAction("", this);
    lineAction->setText(tr("Co&nnect"));
    lineAction->setStatusTip(tr("Connect 2 components with a wire."));
    lineAction->setIcon(QIcon(":/images/linepointer.png"));
    lineAction->setCheckable(true);
    lineAction->setShortcut(Qt::Key_L);
    connect(lineAction, &QAction::triggered, this, [this]{ pointerGroupClicked(lineAction); });

    runningAction = new QAction("", this);
    runningAction->setText(tr("&Running"));
    runningAction->setStatusTip(tr("Sets whether the simulation is running."));
    runningAction->setIcon(QIcon(":/images/start.png"));
    runningAction->setCheckable(true);
    runningAction->setChecked(true);
    runningAction->setShortcut(Qt::Key_R);

    scaleAction = new QWidgetAction(nullptr);
    QWidget* hboxwidget = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    hboxwidget->setLayout(hbox);
    QComboBox* scaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    scaleCombo->addItems(scales);
    scaleCombo->setCurrentText("100%");
    hbox->addWidget(new QLabel("Scale"));
    hbox->addWidget(scaleCombo);
    scaleAction->setDefaultWidget(hboxwidget);

    connect(scaleCombo, &QComboBox::currentTextChanged,this, &MainWindow::sceneScaleChanged);
}


void MainWindow::createMenubar() {
    // Add the different actions to the menubar.

    mainMenuBar = menuBar();

    // Create file menu, add save, open, import, export, exit.
    fileMenu = mainMenuBar->addMenu("&File");
    fileMenu->addAction(saveAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(importAction);
    fileMenu->addAction(exportAction);
    fileMenu->addAction(exitAction);

    itemMenu = mainMenuBar->addMenu("&Item");
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();

    aboutMenu = mainMenuBar->addMenu("&Help");
    aboutMenu->addAction(aboutAction);
    aboutMenu->addAction(tutorialAction);
    aboutMenu->addAction(saveDirAction);

    // If we enabled the hamburger menu at compile-time, add it.
#if HAMBURGER_MENU
    // Hide the traditional menubar. Not deleted as this is still exported to global menus.
    mainMenuBar->setHidden(true);

    // Create hamburger menu
    mainMenu = new HamburgerMenu();

    scaleAction->setParent(mainMenu->menu);
    mainMenu->menu->addAction(scaleAction);

    // Add actions to menu in order.
    mainMenu->menu->addAction(saveAction);
    mainMenu->menu->addAction(openAction);
    mainMenu->menu->addAction(importAction);
    mainMenu->menu->addAction(exportAction);
    mainMenu->menu->addAction(deleteAction);

    mainMenu->menu->addAction(saveDirAction);
    mainMenu->menu->addAction(aboutAction);
    mainMenu->menu->addAction(tutorialAction);

    mainMenu->menu->addAction(exitAction);
#endif
}


void MainWindow::createToolbar() {
    pointerToolbar = addToolBar("Main Toolbar");

    pointerToolbar->setMovable(false);

    pointerToolbar->addAction(moveAction);
    pointerToolbar->addAction(lineAction);
    pointerToolbar->addWidget(new ExpandingSpacer());
    pointerToolbar->addAction(runningAction);
    pointerToolbar->addWidget(new ExpandingSpacer());

#if HAMBURGER_MENU
    pointerToolbar->addWidget((QToolButton*) mainMenu);
#endif

    SHOW_ICON(pointerToolbar, moveAction)
    SHOW_ICON(pointerToolbar, lineAction)
    SHOW_ICON(pointerToolbar, runningAction)
}


template<class T>
QWidget *MainWindow::createCellWidget(std::string text) {
    // Initialise the widget to get the pixmap.
    T item;
    QIcon icon(item.getPixmap());

    // Make a 50*50 icon button for to select the item.
    auto* button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setMinimumSize(10, 10);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setCheckable(true);
    // Add the button to the buttongroup.
    buttonGroup->addButton(button, item.getId());

    // Create a grid with the icon and a label of the icon name.
    auto *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(QString::fromStdString(text)), 1, 0, Qt::AlignCenter);

    // Create a widget for the layout.
    auto* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

template<class T> std::list<T> toStdList(QList<T> in){
    // Turn a QList into a std::list. QT has this by default in Qt 5.12 but I am targeting 5.7 so cannot use this.
    std::list<T> out;

    for(auto i : in){
        out.push_back(i);
    }

    return out;
}

std::string dtos(double in){
	/* Alloc 8 bytes: will take up 4 bytes for the max integer part (1000 max), 2 for the 2dp,
	1 for the point char, and the null byte.*/
	auto outa = (char*) calloc(8, sizeof(char));

	// Write the double to 2 decimal places to the outa char array.
	sprintf(outa, "%.2f", in);

	// Turn the char array to a std::string.
	auto outs = std::string(outa);

	// Free the memory as we copied it to an std::string and no longer need it.
	free((void*) outa);

	return outs;
}

void MainWindow::runSimulation() {
    scene->removeAllText();

    // Is the scene empty? If it is, don't run as this crashes the program.
    if(scene->items().empty()){
        return;
    }

    // If the run button is off then don't run.
    if(! runningAction->isChecked()){
        // Turn off all the components visually.
        for(auto* comp : scene->items()){
            if(IS_TYPE(UIComponent, comp)){
                ((UIComponent*)comp)->setState(false);
            }
        }

        return;
    }

    // Initialise AnalysisMapper with a std::list of scene items.
    AnalysisMapper mapper(toStdList(scene->items()));

    auto sol = mapper.getSolution();

    // Remove all existing text boxes.
    // 'it' is an iterator of a map of  UIComponent: ComponentValue.
    for(auto it : sol){
        std::stringstream ss;

        if (std::to_string(it.second.voltage) != "nan"){ //TODO FIX
            ss << "Voltage: ";
            if(it.second.voltage <  0.01){
                ss << "0";
                it.first->setState(false);

            }else if(it.second.voltage < 1000) {
                ss << dtos(it.second.voltage);
                it.first->setState(true);

            }else{
                ss << "\u221E"; // Infinity symbol unicode escape.
                it.first->setState(true);

            }
            ss << "V";
        }

        if (std::to_string(it.second.current) != "nan"){ //TODO FIX
            ss << "\nCurrent: ";
            if(it.second.current <  0.01){
                ss << "0";
            }else if(it.second.current < 1000) {
                ss << dtos(it.second.current);
            }else{
                ss << "\u221E"; // Infinity symbol unicode escape.
            }
            ss << "A";
        }

        // Create a textbox.
        auto textBox = new SceneText(ss.str());
        auto componentPos = it.first->pos();
        scene->addItem(textBox);
        // Add textbox underneath the component.
        textBox->setPos(QPointF(componentPos.x()+100, componentPos.y()+200));
    }
}

void MainWindow::itemDoubleClicked(UIComponent* item) {
    // Set the contents of the settingsmenu to the settingsbox of the item.
    settingsMenu->setInteriorLayout(item->settingsBox);

    // Open the menu if the menu is not opened.
    if(not settingsMenu->toggleButton->isChecked()){
        settingsMenu->toggleButton->click();
    }
}

void MainWindow::saveScene() {
    std::vector<UIComponent*> components;
    std::vector<Line*> lines;

    // Sort the items into components and lines.
    for(QGraphicsItem *i : scene->items()){
        if(IS_TYPE(UIComponent, i)) {
            // If it is a component, cast and add to components.
            components.push_back((UIComponent*) i);
        } else if(IS_TYPE(Line, i)) {
            // If it is a line, cast and add to lines.
            lines.push_back((Line*) i);
        }
    }

    // Is there already a circuit open? If not name this circuit.
    if(currentOpenedCircuit.empty()){
        // Prompt the user for the name of the circuit to save.
        std::string name = QInputDialog::getText(this, "Circuit Name", "Name").toStdString();
        currentOpenedCircuit = name;
    }

    // If the user still didn't name the circuit then quit the save.
    if(currentOpenedCircuit.empty()){
        return;
    }

    CircuitSaver::saveCircuit(currentOpenedCircuit, SceneItems{components, lines});

    // Indicate to the user the name of the open file.
    setWindowTitle(("Circuit Simulator: " + currentOpenedCircuit).c_str());
}

void MainWindow::openScene() {
    // Prompt the user for the name of the circuit to open.

    auto files = FileUtils::getSaveFiles();

    QStringList items;
    for(auto file : files){
        items << QString::fromStdString(file);
    }

    bool ok;

    std::string name = QInputDialog::getItem(this, "Source", "Select a file to open", items, 0, false, &ok).toStdString();

    CircuitSaver::loadCircuit(name, scene);
    currentOpenedCircuit = name;

    // Indicate to the user the name of the open file.
    setWindowTitle(QString::fromStdString("Circuit Simulator: " + currentOpenedCircuit));
}


void MainWindow::importScene() {
    // Get the file which the user wants to load.
    std::string toImport = QFileDialog::getOpenFileName(this, "Source", ".", "Circuit Files (*.cir)").toStdString();

    // Load that file.
    CircuitSaver::importCircuit(toImport);
}

void MainWindow::exportScene() {
    // Save the scene if not already saved.
    saveScene();

    // Get the path where the user wants to export the scene.
    auto fileName = QFileDialog::getSaveFileName(this, "Destination", ".", "Circuit Files (*.cir)").toStdString();

    // Export the circuit to that location.
    CircuitSaver::exportCircuit(currentOpenedCircuit, fileName);
}

void MainWindow::openSaveDir() {
#if defined(WIN32)
    ShellExecute(NULL, "open", FileUtils::getSaveDir().c_str(), NULL, NULL, SW_SHOWMINIMIZED);
#elif __APPLE__
	system((std::string("open '")+FileUtils::getSaveDir()+std::string("' &")).c_str());
#elif __linux__
    QDesktopServices::openUrl(QUrl(QString::fromStdString(FileUtils::getSaveDir())));
#else
	std::cerr << "Cannot open save dir on this platform!" << std::endl;
#endif
}

int MainWindow::getMode() {
    return moveAction->isChecked()? Scene::MOVE : Scene::INSERT_LINE;
}

void MainWindow::sceneScaleChanged(const QString &scale) {
    double newScale = scale.leftRef(scale.indexOf(tr("%"))).toDouble() / 100.0;
    setScale(newScale);
}


void MainWindow::setScale(double scale) {
    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(scale, scale);
}
