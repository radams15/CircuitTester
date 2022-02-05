#include "Scene.h"
#include "Line.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <iostream>

#include "MainWindow.h"


Scene::Scene(QObject *parent) : QGraphicsScene(parent){
    // Default to move mode.
    mode = MOVE;

    // No components or lines to place down currently.
    component = nullptr;
    line = nullptr;
}


void Scene::setMode(Mode mode){
    this->mode = mode;
}

void Scene::setItem(UIComponent* c){
    this->component = c;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* evt){
    // Only do anything if there was a left click.
    if (evt->button() == Qt::LeftButton){
        switch (mode) {
            case INSERT_ITEM:
                // Add the item to the scene.
                addItem(component);

                // Set the position of the component to where the mouse click occurred.
                component->setPos(evt->scenePos());

                // Notify others that an item was inserted.
                itemInserted(component);
                break;

            case INSERT_LINE:
                // Create the line
                line = new QGraphicsLineItem(QLineF(evt->scenePos(), evt->scenePos()));

                // Set the pen to be 2px wide and black
                line->setPen(QPen(Qt::black, 2));

                // Add the line.
                addItem(line);
                break;

            default: // Any other mode does nothing.
                break;
        }

        // QGraphicsScene then completes what it normally completes by default.
        QGraphicsScene::mousePressEvent(evt);
    }
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* evt) {
    // Is there anything selected?
    if(! selectedItems().empty()){
        // Get the first selected item.
        auto* item = selectedItems().at(0);

        // Is the item a UIComponent (it could be a line as this causes a crash)
        if(IS_TYPE(UIComponent, item)) {
            itemDoubleClicked((UIComponent *) item);
        }
    }

    QGraphicsScene::mouseDoubleClickEvent(evt);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* evt){
    if (mode == INSERT_LINE && line != nullptr) {
        // While moving the mouse draw the line from start to where the mouse is.
        line->setLine(QLineF(line->line().p1(), evt->scenePos()));
    } else if (mode == MOVE) {
        // Just do what QGraphicsScene normally does.
        QGraphicsScene::mouseMoveEvent(evt);
    }
}



void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* evt){
    if (line != nullptr && mode == INSERT_LINE) {
        // Get all the items at the position of the start of the line.
        auto startItems = items(line->line().p1());
        if (startItems.count() != 0 && startItems.first() == line) {
            // Remove the line as the line starts at the start point of the line obviously.
            startItems.removeFirst();
        }

        // Same as above but with the end point.
        auto endItems = items(line->line().p2());
        if (endItems.count() != 0 && endItems.first() == line) {
            endItems.removeFirst();
        }

        // Remove the temporary line from the drawing phase.
        removeItem(line);
        delete line;


        if (!startItems.empty() && !endItems.empty() && IS_TYPE(SceneItem, startItems.first()) && IS_TYPE(SceneItem, endItems.first()) &&  startItems.first() != endItems.first()) {
            auto* startItem = (SceneItem *) startItems.first();
            auto* endItem = (SceneItem *) endItems.first();

            auto* newLine = new Line(startItem, endItem);
            startItem->addLine(newLine);
            endItem->addLine(newLine);
            addItem(newLine);
            newLine->redraw();
        }
    }

    // Reset the temporary line to nullptr.
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(evt);
}

void Scene::removeAllText() {
    for(auto c : items()){
        // For each item, if it is SceneText remove it.
        if(IS_TYPE(SceneText, c)){
            removeItem(c);
        }
    }
}

void Scene::setSize(int width, int height) {
    // Set class values width, height for reference later in CircuitSaver.
    this->width = width;
    this->height = height;

    // Set scene size to width, height starting at 0, 0 at the top left.
    setSceneRect(QRectF(0, 0, width, height));
}
