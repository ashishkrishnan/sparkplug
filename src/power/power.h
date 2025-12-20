#ifndef POWER_H
#define POWER_H
#include <Arduino.h>
#include "IRelay.h"

class Power {
    private:
        IRelay* _relay;
    public:
        Power(IRelay* r) : _relay(r) {}

        void setup() {
            _relay->configure();
        }

        void triggerPulse() {
            _relay->setHigh();
            delay(500);
            _relay->setLow();
        }
};
#endif
