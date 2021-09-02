#include "Line.h"
#include "SceneItem.h"

#include <QPainter>
#include <QPen>
#include <QtMath>


Line::Line(SceneItem *startItem, SceneItem *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), start(startItem), end(endItem){
    // Create a solid black pen with a width of 2px.
    setPen(QPen(penColour, penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // Can select the item - to delete.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    // Set the z coordinate to -1000 so it is always behind any other components.
    setZValue(-1000);
}

void Line::update(){
    QLineF line(mapFromItem(start, start->startPoint()), mapFromItem(end, end->endPoint()));
    setLine(line);
}


void Line::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    // If they are inside each other don't draw the line.
    if (start->collidesWithItem(end)) {
        return;
    }

    // Make a pen, set the colour
    QPen myPen = pen();
    myPen.setColor(penColour);

    // Set the pen to the painter object.
    painter->setPen(myPen);
    painter->setBrush(penColour);

    // Set the line to draw between the start of the start component and the end of the end component.
    setLine(QLineF(start->startPoint(), end->endPoint()));

    // Draw the line.
    painter->drawLine(line());

    // If it is selected draw a border around the line.
    if (isSelected()) {
        // Set the pen to 1px dashed line.
        painter->setPen(QPen(penColour, 1, Qt::DashLine));
        QLineF myLine = line();

        // Move the line 4px up.
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);

        // Move the line 8px down from 4px up to draw 4px under the main line.
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}

