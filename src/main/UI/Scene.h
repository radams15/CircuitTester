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
    enum Mode { INSERT_ITEM, INSERT_LINE, MOVE_ITEM };

    explicit Scene(QMenu *itemMenu, QObject *parent = nullptr);
    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor itemColor() const { return myItemColor; }
    QColor lineColor() const { return myLineColor; }

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
    bool isItemChange(int type) const;

    UIComponent* component;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont myFont;
    SceneText *textItem;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
};


#endif // DIAGRAMSCENE_H
