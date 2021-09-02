/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "SceneItem.h"

/** @brief Arrow between two components on a Scene.
 *
 */
class Arrow : public QGraphicsLineItem{
public:

    /** @brief Initialiser
     *
     * @param startItem The item to start from.
     * @param endItem The item to end at.
     * @param parent The parent widget (never used but is an argument of QGraphicsLineItem so is kept optional).
     */
    Arrow(SceneItem *startItem, SceneItem *endItem, QGraphicsItem *parent = nullptr);

    /** @brief Simply returns the start item.
     *
     * @return start item.
     */
    inline SceneItem* startItem() const { return start; }

    /** @brief Simply returns the end item.
     *
     * @return end item.
     */
    inline SceneItem* endItem() const { return end; }

    /** @brief Updates the position of the line when items move.
     *
     */
    void update();

protected:
    /** @brief Called by QT to paint the line onto the canvas.
     *
     * @param painter The painter object provided by the Scene.
     * @param option Unused
     * @param widget Unused
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    /** @brief The SceneItem where the line begins from.
     *
     */
    SceneItem* start;

    /** @brief The SceneItem where the line goes to.
     *
     */
    SceneItem* end;

    /** @brief The colour of the line.
     *
     */
    QColor penColour = Qt::black;

    /** @brief The diameter of the line (px).
     *
     */
    double penSize = 2;
};


#endif // ARROW_H
