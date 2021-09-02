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

#include <QtWidgets>
#include <QInputDialog>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <regex>

MainWindow::MainWindow() {
    createActions();
    createToolBox();
    createMenus();

    // Create the scene, set it to the specified size.
    scene = new Scene(this);
    scene->setSceneRect(QRectF(0, 0, CANVAS_SIZE));

    // Resize the window
    resize(WINDOW_SIZE);

    // Call itemInserted when an item is inserted into the scene.
    connect(scene, &Scene::itemInserted,
            this, &MainWindow::itemInserted);

    createToolbars();

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
            scene->setItemType(new Resistor);
            break;
        case UI_BATTERY:
            scene->setItemType(new Battery);
            break;
        case UI_WIRE:
            scene->setItemType(new Wire);
            break;
        case UI_SWITCH:
            scene->setItemType(new Switch);
            break;
        default:
            return;
    }

    scene->setMode(Scene::INSERT_ITEM);
}



void MainWindow::deleteItem() {
    // First delete all the arrows.
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
    // Start simple info box
    QMessageBox::about(this, tr("About Circuit Simulator"),
                       tr("This is a circuit simulation program written by Rhys Adams (2021-22)"));
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
    layout->addWidget(createCellWidget<Resistor>(tr("Resistor")), 0, 0);
    layout->addWidget(createCellWidget<Battery>(tr("Battery")), 0, 1);
    layout->addWidget(createCellWidget<Wire>(tr("Wire")), 1, 0);
    layout->addWidget(createCellWidget<Switch>(tr("Switch")), 1, 1);

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
}


void MainWindow::createMenus() {
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
    aboutMenu->addAction(aboutAction);
}


void MainWindow::createToolbars() {
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
QWidget *MainWindow::createCellWidget(const QString &text) {
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
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

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

void MainWindow::itemRightClicked(UIComponent* item) {
    // Set the contents of the settingsmenu to the settingsbox of the item.
    settingsMenu->setInteriorLayout(item->settingsBox);

    // Open the menu if the menu is not opened.
    if(not settingsMenu->toggleButton->isChecked()){
        settingsMenu->toggleButton->click();
    }
}

void MainWindow::saveScene() {
    std::vector<UIComponent*> components;
    std::vector<Line*> arrows;

    for(QGraphicsItem *i : scene->items()){
        if(IS_TYPE(UIComponent, i)) {
            components.push_back((UIComponent*) i);
        } else if(IS_TYPE(Line, i)) {
            arrows.push_back((Line*) i);
        }
    }

    if(currentOpenedCircuit.empty()){
        std::string name = QInputDialog::getText(this, tr("Circuit Name"), tr("Name")).toStdString();
        currentOpenedCircuit = name;
    }

    if(currentOpenedCircuit.empty()){
        return;
    }

    CircuitSaver::saveCircuit(currentOpenedCircuit, SceneItems{components, arrows});
}

void MainWindow::openScene() {
    std::string name = QInputDialog::getText(this, tr("Circuit Name"), tr("Name")).toStdString();
    currentOpenedCircuit = name;
    CircuitSaver::loadCircuit(name, scene);
}


//TODO move all this to the SaveLoad class
void copyFile(std::string src, std::string dst){
    std::ifstream in(src);
    std::ofstream out(dst);

    out << in.rdbuf();

    in.close();
    out.close();
}

bool endsWith(std::string fullString, std::string ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void MainWindow::importScene() {
    std::string toImport = QFileDialog::getOpenFileName(this, tr("Source"), ".", tr("Circuit Files (*.cir)")).toStdString();

    std::regex nameRegex(R"(.*(\/|\\)(.*).cir)");
    std::cmatch match;
    if(! std::regex_search(toImport.c_str(), match, nameRegex)){
        std::cerr << "Cannot regex match: " << toImport << std::endl;
        return;
    }

    if(match[2] != ""){
        auto saveFile = CircuitSaver::getPath(match[2]);
        copyFile(toImport, saveFile);
    }
}

void MainWindow::exportScene() {
    saveScene();
    auto saveFile = CircuitSaver::getPath(currentOpenedCircuit);

    auto fileName = QFileDialog::getSaveFileName(this, tr("Destination"), ".", tr("Circuit Files (*.cir)")).toStdString();

    if(! fileName.empty()){
        if(!endsWith(fileName, ".cir")){
            fileName += ".cir";
        }
        copyFile(saveFile, fileName);
    }
}
