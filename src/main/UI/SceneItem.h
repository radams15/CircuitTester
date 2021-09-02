/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <vector>
#include <QIcon>

class Line;

class SceneItem : public QGraphicsPixmapItem {
public:

    SceneItem(std::string resourcePath, QGraphicsItem *parent = nullptr);

    void removeArrow(Line *arrow);
    void removeArrows();

    inline QPixmap getPixmap() { return pixmap; }

    void addArrow(Line *arrow);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    QPixmap image(){ return pixmap; }

    QPointF centerpoint();
    QPointF startPoint();
    QPointF endPoint();

    std::vector<Line *> arrows;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    QPixmap pixmap;
    std::string resourcePath;

private:
    QPolygonF myPolygon;

    void loadPolygon(QString path);
};


#endif // DIAGRAMITEM_H
