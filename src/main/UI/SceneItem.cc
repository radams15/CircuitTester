#include "SceneItem.h"
#include "Line.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QPainter>
#include <QtGui/QMessageBox>
#include <iostream>
#include <cmath>

#define GRID_SNAP_STEP 20
#define SCENEITEM_SIZE 200, 200

SceneItem::SceneItem(std::string resourcePath, QGraphicsItem* parent) : QGraphicsPixmapItem(parent) {
    // Create pixmap from resource path.
    this->pixmap = QPixmap(QString::fromStdString(resourcePath));

    // Scale the pixmap to defined size, because this greatly helps to determine where the
    // centre and ends of the component are.
    pixmap = pixmap.scaled(SCENEITEM_SIZE);

    // Set the pixmap of this item.
    setPixmap(this->pixmap);

    // Allow movement and selection.
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}


void SceneItem::removeLine(Line* line){
    // Delete all from lines list which are the passed line pointer.
    //TODO fix erase --V
    //lines.erase(std::remove(lines.begin(), lines.end(), line), lines.end());
}


void SceneItem::removeLines(){
    // need a copy here since removeLine() modifies the lines list
    std::vector<Line*> linesCopy = lines;
    foreach(Line* line, linesCopy) {
        // Remove the line from the start and end items.
        line->startItem()->removeLine(line);
        line->endItem()->removeLine(line);

        // Remove the line from the Scene.
        scene()->removeItem(line);
        delete line;
    }
}


void SceneItem::addLine(Line* line){
    // Append to the lines list.
    lines.push_back(line);
}

QVariant SceneItem::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemPositionChange) {
        // Update all the lines attached to this item.
        foreach (Line* arrow, lines){
            arrow->update();
        }
    }

    // Return the value parameter to caller as it is unused.
    return value;
}

QPointF SceneItem::centrePoint() {
    // Get width and height of the image.
    double w = pixmap.width();
    double h = pixmap.height();

    QPointF p = pos();

    // Get x, y coordinates of this SceneItem.
    double x = p.x();
    double y = p.y();

    // Find the centre of the SceneItem by adding half the width to the x and adding half the height to the y.
    double xc = x + (w/2);
    double yc = y + (h/2);

    return QPointF(xc, yc);
}

void SceneItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    double newX = x();
    double newY = y();

    // Round x and y to GRID_SNAP_STEP pixels.
    if((int) newX / GRID_SNAP_STEP != newX / (float)GRID_SNAP_STEP){
        newX= GRID_SNAP_STEP * floor(newX / GRID_SNAP_STEP);
    }

    if((int)newY / GRID_SNAP_STEP != newY / (float)GRID_SNAP_STEP){
        newY= (float)GRID_SNAP_STEP * floor(newY / (float)GRID_SNAP_STEP);
    }

    // Change to the rounded pixels
    setPos(newX, newY);

    // Continue mouseReleaseEvent parent method.
    QGraphicsItem::mouseReleaseEvent(event);
}

QPointF SceneItem::startPoint() {
    QPointF centre = centrePoint();

    // Start is the centre x - half the width.

    return QPointF(centre.x() - (pixmap.width() / 2), centre.y());
}

QPointF SceneItem::endPoint() {
    QPointF centre = centrePoint();

    // Start is the centre x + half the width.

    return QPointF(centre.x() + (pixmap.width() / 2), centre.y());
}

void SceneItem::setColour(QColor colour) {
    QImage tmp = pixmap.toImage();

    // Iterate over every pixel in the image.
    for(int y1 = 0; y1 < tmp.height(); y1++) {
        for (int x1 = 0; x1 < tmp.width(); x1++) {

            // If the pixel is not completely transparent.
            if(qAlpha(tmp.pixel(x1, y1)) > 0) {

                colour.setAlpha(qAlpha(tmp.pixel(x1,y1)));

                // Set the pixel to the new colour.
                tmp.setPixel(x1, y1, colour.rgb());
            }
        }
    }

    // Turn back into pixmap
    pixmap = QPixmap::fromImage(tmp);

    // Refresh the image that is drawn when moved.
    setPixmap(pixmap);
}

std::vector<Line *> SceneItem::leavingLines() {
    std::vector<Line*> out;
    foreach(Line* l, lines){
        if(l->startItem() == this){
            out.push_back(l);
        }
    }

    return out;
}

std::vector<Line *> SceneItem::enteringLines() {
    std::vector<Line*> out;
    foreach(Line* l, lines){
        if(l->endItem() == this){
            out.push_back(l);
        }
    }

    return out;
}
