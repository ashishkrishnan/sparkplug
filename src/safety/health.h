#ifndef HEALTH_H
#define HEALTH_H
#include "ISystemHealth.h"
#include <Arduino.h>
#include <ESPping.h>

class Health : public ISystemHealth {
    public:
        float getInternalTemp() override {
            return temperatureRead();
        }

        bool isTargetPcAlive(const char *ip) override {
            IPAddress addr;
            addr.fromString(ip);
            return Ping.ping(addr);
        }
};
#endif //HEALTH_H
