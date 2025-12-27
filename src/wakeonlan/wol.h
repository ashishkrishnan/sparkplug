#ifndef WOL_H
#define WOL_H
#include <Arduino.h>
#include <WiFiUdp.h>

#include "logger/EventLogger.h"

typedef void (*WakeCallback)(String os, String strategy);

class Wol {
private:
    WiFiUDP Udp;
    WakeCallback onWakeUp;
    // Helper
    byte packetBuffer[102];

    EventLogger logger;

    bool isThermalUnsafe();

public:
    Wol();

    void setupWol(WakeCallback onWakeUpCallback);

    void handleWolLoop();
};

#endif //WOL_H
