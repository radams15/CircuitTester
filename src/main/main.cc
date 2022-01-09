/** @file main.cpp
 * This is the main entry point of the program.
 */
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
    // Load all the images from the resources folder.
    Q_INIT_RESOURCE(resources);

    QApplication a(argc, argv);

    KAboutData aboutData(
            // The program name used internally. (componentName)
            "CircuitTester",
            // A displayable program name string. (displayName)
            "Circuit Simulator",
            // The program version string. (version)
            "1.0",
            // Short description of what the app does. (shortDescription)
            "A simple circuit simulation program.",
            // The license this code is released under
            KAboutLicense::GPL_V2,
            // Copyright Statement (copyrightStatement = QString())
            "(c) 2021-22",
            // Optional text shown in the About box.
            // Can contain any information desired. (otherText)
            "",
            // The program homepage string. (homePageAddress = QString())
            "https://therhys.co.uk/",
            // The bug report email address
            // (bugsEmailAddress = QLatin1String("submit@bugs.kde.org")
            "rhys@therhys.co.uk");

    aboutData.addAuthor("Rhys Adams", "Author", "rhys@therhys.co.uk",
                        "https://therhys.co.uk/");

    KAboutData::setApplicationData(aboutData);

    // Set window icon to the connector image.
    QApplication::setWindowIcon(QIcon(":/images/linepointer.png"));

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
