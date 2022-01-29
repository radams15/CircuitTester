//
// Created by rhys on 1/14/22.
//

#ifndef CIRCUITTESTER_EXPANDINGSPACER_H
#define CIRCUITTESTER_EXPANDINGSPACER_H


#include <QtGui/QWidget>

class ExpandingSpacer : public QWidget{
public:
    inline ExpandingSpacer() : QWidget(){setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);}
};

#endif //CIRCUITTESTER_EXPANDINGSPACER_H
