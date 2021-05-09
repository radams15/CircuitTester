/** @file main.cpp
 * This is the main entry point of the program.
 */
#include <iostream>


#include <QApplication>

#include "UI/mainwindow.h"

#include "Analysis/MNAElement.h"
#include "Analysis/MNASolution.h"
#include "Analysis/MNACircuit.h"

#include "Components/Battery.h"
#include "Components/Resistor.h"


/** @brief Main GUI method.
 * This method runs the QT GUI code and
 * returns the status code of it.
 */
int guiMain(int argc, char** argv){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}

/** @brief Main method.
 * This is the main program entry point.
 */
int main(int argc, char** argv){
    auto bat1 = new Battery(0, 1, -4);
    auto bat2 = new Battery(1, 2, -4);
    auto res = new Resistor(2, 0, 2);

    auto cir = new MNACircuit({bat1, bat2, res});

    std::map<int, double> vmap = {
            {0, 0.0},
            {1, -4.0},
            {2, -8.0},
    };

    auto dessol = new MNASolution(vmap, {bat1->withCurrentSolution(-4), bat2->withCurrentSolution(-4)});

    auto sol = cir->solve();

    if(sol->equals(*dessol)){
        std::cout << "Good\n";
    }else{
        std::cout << "Bad!\n";
    }

    for(auto v : sol->voltageMap){
        std::cout << v.first << ": " << v.second << std::endl;
    }

    return 0;
}
