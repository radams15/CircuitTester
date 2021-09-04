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

/** @brief The scene class is a QGraphicsScene derivitive that holds the components
 * in the circuit.
 *
 */
class Scene : public QGraphicsScene{
    Q_OBJECT

public:
    /** @brief The current mode of the scene.
     *
     */
    enum Mode {
        /** @brief Item being placed down.
         *
         */
        INSERT_ITEM,
        /** @brief Line being placed down.
         *
         */
        INSERT_LINE,

        /** @brief Item being moved.
         *
         */
        MOVE
    };

    int width, height;

    /** @brief Initialises the scene.
     *
     * @param parent The parent widget - unused but argument of by QGraphicsScene.
     */
    explicit Scene(QObject* parent = nullptr);

    /** @brief Sets the canvas size.
     *
     * @param width Width of canvas.
     * @param height Height of canvas.
     */
    void setSize(int width, int height);

    /** @brief Removes all text boxes from the scene.
     *
     */
    void removeAllText();

public slots:
    /** @brief Setter for mode.
     *
     * @param mode The new mode.
     */
    void setMode(Scene::Mode mode);

    /** @brief Sets the new component to be placed down.
     *
     * @param c A pointer to a new UIComponent to be placed down.
     */
    void setItemType(UIComponent* c);

signals:
    /** @brief Called when an item is inserted into the scene.
     *
     * @param c The pointer to the UIComponent that was inserted.
     */
    void itemInserted(UIComponent* c);

    /** Called when any text is inserted into the scene.
     *
     * @param item The QGraphicsTextItem that was inserted.
     */
    void textInserted(QGraphicsTextItem* item);

    /** Called when any part of the scene was selected.
     *
     * @param item The QGraphicsItem that was selected.
     */
    void itemSelected(QGraphicsItem* item);

protected:
    /** @brief Called when the mouse is pressed once.
     *
     * Called to handle selecting an item or placing an item down.
     *
     * @param mouseEvent The event that is created when an item is placed down.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

    /** @brief Called when the mouse is pressed twice.
     *
     * Called to handle accessing the settings menu of a component.
     *
     * @param mouseEvent
     */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

    /** @brief Called when the mouse moves.
     *
     * Called to handle moving an item or placing down a line.
     *
     * @param mouseEvent
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

    /** @brief Called when the mouse is released.
     *
     * Called to handle finishing placing a line.
     *
     * @param mouseEvent
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

private:
    /** @brief The component that will be placed down.
     *
     * Is nullptr if there is no component to place down.
     *
     */
    UIComponent* component;

    /** @brief The line that is temporarily shown when drawing a line.
     *
     * Is nullptr if there is no line being drawn.
     *
     */
    QGraphicsLineItem* line;

    /** @brief The mode that the scene is currently in.
     *
     */
    Mode currentMode;
};


#endif // DIAGRAMSCENE_H
