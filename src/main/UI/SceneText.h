/**
 * @author Rhys Adams
 * @date 29/01/2021
 */

#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include <string>

/** @brief Places text on the screen.
 *
 * Simple wrapper that sets the text as std::string through the initialiser.
 *
 */
class SceneText : public QGraphicsTextItem {
    Q_OBJECT

public:
    /** @brief Initialiser.
     *
     * @param text The text to show.
     */
    explicit SceneText(const std::string text);
};


#endif // DIAGRAMTEXTITEM_H
