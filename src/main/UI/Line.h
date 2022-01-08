/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef LINE_H
#define LINE_H

#include <QGraphicsLineItem>

#include "SceneItem.h"

/** @brief Line between two components on a Scene.
 *
 */
class Line : public QGraphicsPathItem{
public:

    /** @brief Initialiser
     *
     * @param startItem The item to start from.
     * @param endItem The item to end at.
     */
    Line(SceneItem *startItem, SceneItem *endItem);

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


#endif // LINE_H
