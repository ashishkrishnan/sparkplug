#ifndef MOCKHEALTH_H
#define MOCKHEALTH_H
#include <Arduino.h>

class MockHealth {
    public:
        float fakeTemp = 40.0;
        bool fakePing = true;

        float getInternalTemp() {
            return fakeTemp;
        }

        bool isTargetPcAlive() {
            return fakePing;
        }
};

#endif //MOCKHEALTH_H
