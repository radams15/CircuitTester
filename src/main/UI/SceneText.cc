#include "SceneText.h"
#include "Scene.h"


SceneText::SceneText(const std::string text) : QGraphicsTextItem(NULL){
    // Set the text to the text param by converting to QString.
    setPlainText(QString::fromStdString(text));
}
