#ifndef MOCKHEALTH_H
#define MOCKHEALTH_H
#include <Arduino.h>
#include "safety/ISystemHealth.h"

class MockHealth : public ISystemHealth {
    public:
        float fakeTemp = 40.0;
        bool fakePing = true;

        float getInternalTemp() override {
            return fakeTemp;
        }

        bool isTargetPcAlive(const char *ip) override {
            return fakePing;
        }
};

#endif //MOCKHEALTH_H
