#include "SceneItem.h"
#include "Line.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QResource>
#include <QMessageBox>
#include <iostream>
#include <cmath>

#define GRID_SNAP_STEP 20


SceneItem::SceneItem(std::string resourcePath, QGraphicsItem* parent) : QGraphicsPixmapItem(parent) {
    this->resourcePath = resourcePath;
    this->pixmap = QPixmap(QString::fromStdString(resourcePath));
    pixmap = pixmap.scaled(200, 200);
    setPixmap(this->pixmap);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}


void SceneItem::removeArrow(Line *arrow){
    arrows.erase(std::remove(arrows.begin(), arrows.end(), arrow), arrows.end());
}


void SceneItem::removeArrows(){
    // need a copy here since removeArrow() will
    // modify the arrows container
    const auto arrowsCopy = arrows;
    for (Line *arrow : arrowsCopy) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}


void SceneItem::addArrow(Line* arrow){
    arrows.push_back(arrow);
}

QVariant SceneItem::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemPositionChange) {
        for (Line* arrow : arrows){
            arrow->update();
        }
    }

    return value;
}

QPointF SceneItem::centerpoint() {
    double w = pixmap.width();
    double h = pixmap.height();

    QPointF p = pos();

    double x = p.x();
    double y = p.y();

    double xc = x + (w/2);
    double yc = y + (h/2);

    return {xc, yc};
}

void SceneItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    double newX = x();
    double newY = y();

    if((int) newX / GRID_SNAP_STEP != newX / (float)GRID_SNAP_STEP){
        newX= 5.0 * round(newX / 5.);
    }

    if((int)newY / GRID_SNAP_STEP != newY / (float)GRID_SNAP_STEP){
        newY= (float)GRID_SNAP_STEP * round(newY / (float)GRID_SNAP_STEP);
    }

    setPos(newX, newY);
    QGraphicsItem::mouseReleaseEvent(event);
}

QPointF SceneItem::startPoint() {
    auto center = centerpoint();

    return QPointF(center.x()-(pixmap.width()/2), center.y());
}

QPointF SceneItem::endPoint() {
    auto center = centerpoint();

    return QPointF(center.x()+(pixmap.width()/2), center.y());
}
