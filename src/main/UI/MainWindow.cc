#include <vector>
#include <queue>

#include "Line.h"
#include "SceneItem.h"
#include "Scene.h"
#include "SceneText.h"
#include "MainWindow.h"

#include "Components/Resistor.h"
#include "Components/Battery.h"
#include "Components/Wire.h"
#include "Components/Switch.h"

#include "../Saves/CircuitSaver.h"
#include "AnalysisMapper.h"

#include "../Saves/FileUtils.h"

#include <QtWidgets>
#include <QInputDialog>
#include <iostream>
#include <iomanip>
#include <sstream>

MainWindow::MainWindow() {
    createActions();
    createToolBox();
    createMenubar();
    
    // Set window icon to the connector image.
    setWindowIcon(QIcon(":/images/linepointer.png"));

    // Create the scene, set it to the specified size.
    scene = new Scene(this);
    scene->setSize(CANVAS_SIZE);

    // Resize the window
    resize(WINDOW_SIZE);

    // Call itemInserted when an item is inserted into the scene.
    connect(scene, &Scene::itemInserted,
            this, &MainWindow::itemInserted);

    createToolbar();

    // Create the main layout, add the graphics view.
    auto* layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    // Create and add the settings menu.
    settingsMenu = new SettingsMenu();
    layout->addWidget(settingsMenu);

    // Create a widget to hold the layout.
    auto *widget = new QWidget;
    widget->setLayout(layout);

    // Set the widget to the centre of the window.
    setCentralWidget(widget);
    setWindowTitle(tr("Circuit Simulator"));

    // Makes the toolbar on mac look the same colour as the titlebar - just aesthetic.
    setUnifiedTitleAndToolBarOnMac(true);

    auto* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(runSimulation()));
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



void MainWindow::pointerGroupClicked() {
    // Selected either line or move mode, set the mode.
    scene->setMode(Scene::Mode(pointerTypeGroup->checkedId()));
}


void MainWindow::itemInserted(UIComponent* c) {
    // Check the move mode button.
    pointerTypeGroup->button((int) Scene::MOVE)->setChecked(true);

    // Set the mode to move mode.
    scene->setMode(Scene::Mode((int) Scene::MOVE));

    // Uncheck the button of that component.
    buttonGroup->button(c->getId())->setChecked(false);
}

void MainWindow::about() {
    // Start simple about box
    QMessageBox::about(this, tr("About Circuit Simulator"),
                       tr("This is a circuit simulation program written by Rhys Adams (2021-22)"));
}

void MainWindow::tutorial() {
    // Start simple about box
    QMessageBox::about(this, tr("Tutorial"),
                       tr("Welcome to Rhys' circuit simulation program.\n\nHere is a brief tutorial to understand how the program works.\n\nTo place down a component, first click on the component button, then click on the location\non the canvas where you want the component to go.\n\nYou can move components by dragging the black parts.\n\nAt the top left, you can either select movement mode (the mouse pointer button) to drag components,\nor the connector button (the black line to the left of the mouse pointer) to connect components\nwith lines.\n\nDouble-clicking any component will open up the settings menu on the right, which allows you to\nchange anything about any of the Components, such as voltage, resistance or whether they are on or off.\n\nYou can reach this info again at the help->Tutorial menu."));
}


void MainWindow::createToolBox() {
    // Create the button group.
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);

    // When the button group is clicked, call buttonGroupClicked.
    connect(buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
            this, &MainWindow::buttonGroupClicked); // https://doc.qt.io/archives/qt-5.6/qbuttongroup.html#buttonClicked

    // Create new grid layout, add the widgets for each component type to a 2*n grid.
    auto* layout = new QGridLayout;
    layout->addWidget(createCellWidget<Resistor>("Resistor"), 0, 0);
    layout->addWidget(createCellWidget<Battery>("Battery"), 0, 1);
    layout->addWidget(createCellWidget<Wire>("Wire"), 1, 0);
    layout->addWidget(createCellWidget<Switch>("Switch"), 1, 1);

    // Don't stretch completely, just 3 pixels stretch per row.
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    // Create a widget to hold the layout, add the widget.
    auto* itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    // Create a toolbox to hold the components.
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Components"));
}


void MainWindow::createActions() {
    // Create many actions which can be placed in menubars or toolbars.

    // The ambersand is placed before the letter that is used for alt-navigation.
    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);

    exitAction = new QAction(tr("E&xit"), this);
    // Shortcut is the OS quit key: command-q for mac, control-q for linux/windows.
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit the circuit simulator"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    runAction = new QAction(tr("&Run"), this);
    runAction->setShortcut(tr("F5"));
    runAction->setStatusTip(tr("Run the circuit"));
    connect(runAction, &QAction::triggered, this, &MainWindow::runSimulation);

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    tutorialAction = new QAction(tr("&Tutorial"), this);
    tutorialAction->setShortcut(tr("F2"));
    connect(tutorialAction, &QAction::triggered, this, &MainWindow::tutorial);

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveScene);

    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(tr("Ctrl+O"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openScene);

    importAction = new QAction(tr("&Import"), this);
    importAction->setShortcut(tr("Ctrl+I"));
    connect(importAction, &QAction::triggered, this, &MainWindow::importScene);

    exportAction = new QAction(tr("&Export"), this);
    exportAction->setShortcut(tr("Ctrl+E"));
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportScene);

    saveDirAction = new QAction(tr("Open Save Directory"), this);
    connect(saveDirAction, &QAction::triggered, this, &MainWindow::openSaveDir);
}


void MainWindow::createMenubar() {
    // Add the different actions to the menubar.

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(importAction);
    fileMenu->addAction(exportAction);
    fileMenu->addAction(exitAction);

    simMenu = menuBar()->addMenu(tr("&Simulation"));
    simMenu->addAction(runAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(saveDirAction);
    aboutMenu->addAction(aboutAction);
    aboutMenu->addAction(tutorialAction);
}


void MainWindow::createToolbar() {
    // Create button for the scene move mode.
    auto *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    // Set the move mode default on.
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    auto *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, (int) Scene::MOVE);
    pointerTypeGroup->addButton(linePointerButton, (int) Scene::INSERT_LINE);

    connect(pointerTypeGroup, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
            this, &MainWindow::pointerGroupClicked);  // https://doc.qt.io/archives/qt-5.6/qbuttongroup.html#buttonClicked

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);

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
    button->setCheckable(true);
    // Add the button to the buttongroup.
    buttonGroup->addButton(button, item.getId());

    // Create a grid with the icon and a label of the icon name.
    auto *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr(text.c_str())), 1, 0, Qt::AlignCenter);

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

void MainWindow::runSimulation() {
    // Is the scene empty? If it is, don't run as this crashes the program.
    if(scene->items().empty()){
        return;
    }

    // Initialise AnalysisMapper with a std::list of scene items.
    AnalysisMapper mapper(toStdList(scene->items()));

    auto sol = mapper.getSolution();

    // Remove all existing text boxes.
    scene->removeAllText();

    // 'it' is a map of  UIComponent: ComponentValue.
    for(auto it : sol){
        std::stringstream ss;
        ss << std::setprecision(2);

        ss << "Voltage: ";
        ss << it.second.voltage;
        ss << "V";

        if (std::to_string(it.second.current) != "nan"){ //TODO FIX THIS MONSTROSITY
            ss << "\nCurrent: ";
            if(it.second.current < 1000){
                ss << it.second.current;
            }else if(it.second.current < 0.01) {
                ss << 0;
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
        std::string name = QInputDialog::getText(this, tr("Circuit Name"), tr("Name")).toStdString();
        currentOpenedCircuit = name;
    }

    // If the user still didn't name the circuit then quit the save.
    if(currentOpenedCircuit.empty()){
        return;
    }

    CircuitSaver::saveCircuit(currentOpenedCircuit, SceneItems{components, lines});

    // Indicate to the user the name of the open file.
    setWindowTitle(tr(("Circuit Simulator: " + currentOpenedCircuit).c_str()));
}

void MainWindow::openScene() {
    // Prompt the user for the name of the circuit to open.
    //std::string name = QInputDialog::getText(this, tr("Circuit Name"), tr("Name")).toStdString();

    auto files = FileUtils::getSaveFiles();

    QStringList items;
    for(auto file : files){ items << QString::fromStdString(file); }

    bool ok;

    std::string name = QInputDialog::getItem(this, tr("Source"), tr("Select a file to open"), items, 0, false, &ok).toStdString();

    CircuitSaver::loadCircuit(name, scene);
    currentOpenedCircuit = name;

    // Indicate to the user the name of the open file.
    setWindowTitle(tr(("Circuit Simulator: " + currentOpenedCircuit).c_str()));
}


void MainWindow::importScene() {
    // Get the file which the user wants to load.
    std::string toImport = QFileDialog::getOpenFileName(this, tr("Source"), ".", tr("Circuit Files (*.cir)")).toStdString();

    // Load that file.
    CircuitSaver::importCircuit(toImport);
}

void MainWindow::exportScene() {
    // Save the scene if not already saved.
    saveScene();

    // Get the path where the user wants to export the scene.
    auto fileName = QFileDialog::getSaveFileName(this, tr("Destination"), ".", tr("Circuit Files (*.cir)")).toStdString();

    // Export the circuit to that location.
    CircuitSaver::exportCircuit(currentOpenedCircuit, fileName);
}

void MainWindow::openSaveDir() {
    // TODO Fix open save directory button.
    // Opens the save directory in file explorer. Not working on mac/windows yet.
    QDesktopServices::openUrl(QString::fromStdString(FileUtils::getSaveDir()));
}
