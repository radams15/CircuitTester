//
// Created by rhys on 09/05/2021.
//

#ifndef CIRCUITTESTER_UNKNOWN_H
#define CIRCUITTESTER_UNKNOWN_H

#include <string>
#include "MNAElement.h"

class Unknown {
private:

public:
    enum Type{
        CURRENT,
        VOLTAGE
    };

    Type t;

    virtual bool equals(Unknown* other) = 0;
    virtual std::string toTermName() = 0;

    Unknown(Type t);
};

class UnknownCurrent : public Unknown {
private:

public:
    MNAElement* element;

    explicit UnknownCurrent(MNAElement* element);

    std::string toTermName() override;
    bool equals(Unknown* other) override;
};

class UnknownVoltage : public Unknown {
private:

public:
    int node;

    explicit UnknownVoltage(int node);

    std::string toTermName() override;
    bool equals(Unknown* other) override;
};


#endif //CIRCUITTESTER_UNKNOWN_H
