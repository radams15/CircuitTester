#include "Line.h"
#include "SceneItem.h"

#include <QPainter>
#include <QPen>
#include <QtMath>


Line::Line(SceneItem *startItem, SceneItem *endItem)
    : QGraphicsLineItem(nullptr) {

    this->start = startItem;
    this->end = endItem;

    // Create a solid black pen with a width of 2px.
    setPen(QPen(penColour, penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // Can select the item to delete it.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    // Set the z coordinate to -1000 so it is always behind any other components.
    setZValue(-1000);
}

void Line::update(){
    // Set the start and end coords of the line.
    setLine(QLineF(start->startPoint(), end->endPoint()));
}


void Line::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    // Make a pen, set the colour
    QPen myPen = pen();
    myPen.setColor(penColour);

    // Set the pen to the painter object.
    painter->setPen(myPen);
    painter->setBrush(penColour);

    update();

    // Draw the line.
    painter->drawLine(line());
}

