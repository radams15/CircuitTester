/** @file main.cpp
 * This is the main entry point of the program.
 */
#include <QDir>
#include <iostream>

#include <QApplication>
#include <KAboutData>
#include "Saves/FileUtils.h"

#include "UI/MainWindow.h"

/** @brief Main GUI method.
 * This method runs the QT GUI code and
 * returns the status code of it.
 */
int guiMain(int argc, char** argv){
    QApplication a(argc, argv);

    // Load all the images from the resources folder.
    Q_INIT_RESOURCE(resources);

    KAboutData aboutData(
            "CircuitTester",
            "Circuit Simulator",
            "1.0",
            "A simple circuit simulation program.",
            KAboutLicense::GPL_V2,
            "(c) 2021-22",
            "",
            "https://github.com/radams15/CircuitTester",
            "rhys@therhys.co.uk");

    aboutData.addAuthor("Rhys Adams", "Author", "rhys@therhys.co.uk",
                        "https://github.com/radams15");

    KAboutData::setApplicationData(aboutData);

    QApplication::setWindowIcon(QIcon(":/images/linepointer.png"));

    QDir::addSearchPath("icon", ":/theme/theme");

    QFile file(":/theme/theme.qss");
    file.open(QFile::ReadOnly);

    QString styleSheet { file.readAll() };
    //a.setStyleSheet(styleSheet);

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
