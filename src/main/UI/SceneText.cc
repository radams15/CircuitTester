#include "SceneText.h"
#include "Scene.h"


SceneText::SceneText(const std::string text, QGraphicsItem *parent) : QGraphicsTextItem(parent){
    setPlainText(QString::fromStdString(text));
}