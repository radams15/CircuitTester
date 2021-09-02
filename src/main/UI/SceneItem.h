/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <vector>
#include <QIcon>

// Define Line as empty class here as Line depends on SceneItem and vice-versa so they cannot include each other.
class Line;

/** @brief An image on the Scene.
 *
 */
class SceneItem : public QGraphicsPixmapItem {
public:

    /** @brief Initialiser
     *
     * @param resourcePath The path to the image of the item.
     * @param parent
     */
    explicit SceneItem(std::string resourcePath, QGraphicsItem *parent = nullptr);

    /** @brief Removes a specified line from lines.
     *
     * @param line Pointer to the line to remove.
     */
    void removeLine(Line *line);

    /** @brief Removes all allows from the item.
     *
     */
    void removeLines();

    /** @brief Returns the image of the SceneItem.
     *
     * Only ever used to set the icon on the MainWindow.
     *
     * @return QPixmap which is the current image of the SceneItem.
     */
    inline QPixmap getPixmap() { return pixmap; }

    /** @brief Add a line that is attached to this SceneItem.
     *
     * @param line Pointer to the line to add.
     */
    void addLine(Line* line);

    /** @brief Called when the mouse is released.
     *
     * Called to handle dropping the SceneItem to a new location.
     *
     * @param event Event of the dropping of the item.
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    /** @brief Gets the centre of the component.
     *
     * @return Centre point of the item.
     */
    QPointF centrePoint();

    /** @brief Gets the start point of the component.
     *
     * @return Start point of the component.
     */
    QPointF startPoint();

    /** @brief Gets the end point of the component.
     *
     * @return End point of the component.
     */
    QPointF endPoint();

    /** @brief All the lines attached to this item.
     *
     */
    std::vector<Line*> lines;

protected:
    /** @brief Item position has changed.
     *
     * @param change Unused.
     * @param value Unused.
     * @return value parameter above.
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /** @brief The image of this component set during initialisation.
     *
     */
    QPixmap pixmap;
};


#endif // DIAGRAMITEM_H
