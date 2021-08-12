#include "Scene.h"
#include "Arrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <iostream>

#include "MainWindow.h"


Scene::Scene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent){

    myItemMenu = itemMenu;
    myMode = MOVE_ITEM;
    component = nullptr;
    line = nullptr;
    textItem = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

}


void Scene::setMode(Mode mode){
    myMode = mode;
}

void Scene::setItemType(UIComponent* c){
    this->component = c;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::LeftButton){
        switch (myMode) {
            case INSERT_ITEM:
                addItem(component);
                component->setPos(mouseEvent->scenePos());
                emit itemInserted(component);
                break;

            case INSERT_LINE:
                line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));

                line->setPen(QPen(myLineColor, 2));

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
    if (myMode == INSERT_LINE && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MOVE_ITEM) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}



void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (line != nullptr && myMode == INSERT_LINE) {
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
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
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
