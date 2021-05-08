#include <iostream>


#include <QApplication>

#include "UI/mainwindow.h"

#include "Analysis/MNAElement.h"
#include "Analysis/MNASolution.h"
#include "Analysis/MNACircuit.h"

#include "Analysis/Components/Battery.h"
#include "Analysis/Components/Resistor.h"


int guiMain(int argc, char** argv){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}

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

    if(sol->approxEquals(*dessol)){
        std::cout << "Good\n";
    }else{
        std::cout << "Bad!\n";
    }

    for(auto v : sol->nodeVoltages){
        std::cout << v.first << ": " << v.second << std::endl;
    }

    return 0;
}
