#include "Line.h"
#include "SceneItem.h"

#include <QPainter>
#include <QPen>
#include <cmath>
#include <iostream>

#define USE_CURVE 0

Line::Line(SceneItem *startItem, SceneItem *endItem)
    : QGraphicsPathItem(nullptr) {

    this->start = startItem;
    this->end = endItem;

    // Create a solid black pen with a width of 2px.
    setPen(QPen(penColour, penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // Set brush to make hollow curve
    setBrush(QBrush(Qt::NoBrush));

    // Can select the item to delete it.
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    // Set the z coordinate to -1000 so it is always behind any other components.
    setZValue(1000);
}

double distance(QPointF a, QPointF b){
    //return sqrt(pow((a.x()-b.x()), 2) + pow((a.y()-b.y()), 2));

    return a.y()-b.y();
}

bool higher(QPointF a, QPointF b){
    return a.y() > b.y();
}

void Line::update(){
    QPointF startp;
    QPointF endp;

    if(start->leavingLines().size() == 1 and end->leavingLines().size() == 1) {
        if (higher(start->endPoint(), end->endPoint())) {
            startp = start->endPoint();
            endp = end->endPoint();
        } else {
            startp = start->startPoint();
            endp = end->startPoint();
        }
    }else{
        startp = start->endPoint();
        endp = distance(startp, end->startPoint()) < distance(startp, end->endPoint())? end->startPoint() : end->endPoint();
    }

    // Create a path with the start point set to the start of the first item.
    QPainterPath path(startp);

    // Get point halfway between the 2 points to act as the control point in the Bézier curve.
    QPointF c(startp.x(), endp.y());

    // Draw quadratic Bézier curve to the endpoint of the end item through point c.
#if USE_CURVE
    path.quadTo(c, endp);
#else
    path.lineTo(c);
    path.lineTo(endp);
#endif

    setPath(path);
}


void Line::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    // Make a pen, set the colour
    QPen myPen = pen();
    myPen.setColor(penColour);

    // Set the pen to the painter object.
    painter->setPen(myPen);

    update();

    // Draw the line.
    painter->drawPath(path());

}

