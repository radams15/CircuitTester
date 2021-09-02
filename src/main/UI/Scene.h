/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include "SceneItem.h"
#include "SceneText.h"

#include <QGraphicsScene>
#include "UIComponent.h"

/**
 * \def IS_TYPE(type, ptr)
 * Attempts to find whether \ptr is an instance of \type through a dynamic cast.
 *
 * If they are not the same class, then a dynamic cast returns a nullptr.
*/

#define IS_TYPE(type, ptr) dynamic_cast<type*>(ptr) != nullptr

class Scene : public QGraphicsScene{
    Q_OBJECT

public:
    enum Mode {
        INSERT_ITEM,
        INSERT_LINE,
        MOVE
    };

    explicit Scene(QObject *parent = nullptr);

    void removeAllText();

public slots:
    void setMode(Scene::Mode mode);
    void setItemType(UIComponent* c);

signals:
    void itemInserted(UIComponent *c);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    UIComponent* component;
    Mode currentMode;
    QPointF startPoint;
    QGraphicsLineItem* line;
};


#endif // DIAGRAMSCENE_H
