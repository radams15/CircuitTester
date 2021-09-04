/** @file main.cpp
 * This is the main entry point of the program.
 */
#include <iostream>

#include <QApplication>
#include "Saves/FileUtils.h"

#include "UI/MainWindow.h"

/** @brief Main GUI method.
 * This method runs the QT GUI code and
 * returns the status code of it.
 */
int guiMain(int argc, char** argv){
    // Load all the images from the resources folder.
    Q_INIT_RESOURCE(resources);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}

/** @brief Main method.
 * This is the main program entry point, which for now just starts the guiMain
 * method and returns the output to the caller.
 */
int main(int argc, char** argv){
    if(!FileUtils::saveDirExists()) {
        FileUtils::createSaveDir();
    }
    return guiMain(argc, argv);
}
