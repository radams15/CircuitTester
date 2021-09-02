/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include <string>

class SceneText : public QGraphicsTextItem {
    Q_OBJECT

public:
    explicit SceneText(const std::string text, QGraphicsItem *parent = nullptr);
};


#endif // DIAGRAMTEXTITEM_H
