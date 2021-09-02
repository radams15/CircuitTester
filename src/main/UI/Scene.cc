#include "Scene.h"
#include "Arrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <iostream>

#include "MainWindow.h"


Scene::Scene(QObject *parent)
    : QGraphicsScene(parent){

    currentMode = MOVE;
    component = nullptr;
    line = nullptr;
}


void Scene::setMode(Mode mode){
    currentMode = mode;
}

void Scene::setItemType(UIComponent* c){
    this->component = c;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::LeftButton){
        switch (currentMode) {
            case INSERT_ITEM:
                addItem(component);
                component->setPos(mouseEvent->scenePos());
                emit itemInserted(component);
                break;

            case INSERT_LINE:
                line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));

                line->setPen(QPen(Qt::black, 2));

                addItem(line);
                break;

            default:
                break;
        }
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if(! selectedItems().empty()){
        auto* item = ((UIComponent*) selectedItems().at(0));
        ((MainWindow*) parent())->itemRightClicked(item);
    }

    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (currentMode == INSERT_LINE && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (currentMode == MOVE) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}



void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (line != nullptr && currentMode == INSERT_LINE) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;


        if (startItems.count() > 0 && endItems.count() > 0 && startItems.first()->type() == SceneItem::Type && endItems.first()->type() == SceneItem::Type &&  startItems.first() != endItems.first()) {
            auto *startItem = qgraphicsitem_cast<SceneItem *>(startItems.first());
            auto *endItem = qgraphicsitem_cast<SceneItem *>(endItems.first());
            auto *arrow = new Arrow(startItem, endItem);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            addItem(arrow);
            arrow->update();
        }
    }
 
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void Scene::removeAllText() {
    for(auto c : items()){
        if(IS_TYPE(SceneText, c)){
            removeItem(c);
        }
    }
}
