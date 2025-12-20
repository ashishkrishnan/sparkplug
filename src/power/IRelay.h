#ifndef IRELAY_H
#define IRELAY_H
#include <Arduino.h>

class IRelay {
    public:
        virtual void configure() = 0;
        virtual void setHigh() = 0;
        virtual void setLow() = 0;
};

#endif