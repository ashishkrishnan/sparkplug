#ifndef RELAY_H
#define RELAY_H
#include "IRelay.h"
#include "Arduino.h"
#include "config/config.h"

class Relay : public IRelay {
    public:
        void configure() override {
            pinMode(PIN_RELAY, OUTPUT);
            setLow();
        }

        void setHigh() override {
            digitalWrite(PIN_RELAY, HIGH);
        }

        void setLow() override {
            digitalWrite(PIN_RELAY, LOW);
        }
};

#endif //RELAY_H
