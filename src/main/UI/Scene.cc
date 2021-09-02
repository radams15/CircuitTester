#include "Scene.h"
#include "Line.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <iostream>

#include "MainWindow.h"


Scene::Scene(QObject *parent)
    : QGraphicsScene(parent){

    // Default to move mode.
    currentMode = MOVE;

    // No components or lines to place down currently.
    component = nullptr;
    line = nullptr;
}


void Scene::setMode(Mode mode){
    currentMode = mode;
}

void Scene::setItemType(UIComponent* c){
    this->component = c;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent){
    // Only do anything if there was a left click.
    if (mouseEvent->button() == Qt::LeftButton){
        switch (currentMode) {
            case INSERT_ITEM:
                // Add the item to the scene.
                addItem(component);

                // Set the position of the component to where the mouse click occured.
                component->setPos(mouseEvent->scenePos());

                // Notify others that an item was inserted.
                emit itemInserted(component);
                break;

            case INSERT_LINE:
                // Create the line
                line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));

                // Set the pen to be 2px wide and black
                line->setPen(QPen(Qt::black, 2));

                // Add the line.
                addItem(line);
                break;

            default: // Any other mode does nothing.
                break;
        }

        // QGraphicsScene then completes what it normally completes by default.
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) {
    // Is there anything selected?
    if(! selectedItems().empty()){
        // Get the first selected item.
        auto* item = selectedItems().at(0);

        // Is the item a UIComponent (it could be a line as this causes a crash)
        if (1){//if(IS_TYPE(UIComponent, item)) {
            ((MainWindow *) parent())->itemRightClicked((UIComponent*) item);
        }
    }

    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (currentMode == INSERT_LINE && line != nullptr) {
        // While moving the mouse draw the line from start to where the mouse is.
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (currentMode == MOVE) {
        // Just do what QGraphicsScene normally does.
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}



void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent){
    if (line != nullptr && currentMode == INSERT_LINE) {
        // Get all the items at the position of the start of the line.
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() != 0 && startItems.first() == line) {
            // Remove the line as the line starts at the start point of the line obviously.
            startItems.removeFirst();
        }

        // Same as above but with the end point.
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() != 0 && endItems.first() == line) {
            endItems.removeFirst();
        }

        // Remove the temporary line from the drawing phase.
        removeItem(line);
        delete line;


        if (startItems.count() > 0 && endItems.count() > 0 && startItems.first()->type() == SceneItem::Type && endItems.first()->type() == SceneItem::Type &&  startItems.first() != endItems.first()) {
            auto* startItem = (SceneItem *) startItems.first();
            auto* endItem = (SceneItem *) endItems.first();

            auto* newLine = new Line(startItem, endItem);
            startItem->addArrow(newLine);
            endItem->addArrow(newLine);
            addItem(newLine);
            newLine->update();
        }
    }

    // Reset the temporary line to nullptr.
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void Scene::removeAllText() {
    for(auto c : items()){
        // For each item, if it is SceneText remove it.
        if(IS_TYPE(SceneText, c)){
            removeItem(c);
        }
    }
}
