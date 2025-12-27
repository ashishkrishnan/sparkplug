#ifndef WOL_H
#define WOL_H
#include <Arduino.h>
#include <WiFiUdp.h>

typedef void (*WakeCallback)(String os, String strategy);

/**
 * Handles Wake-on-lan functionality. In prior versions of sparkplug, it was present in WebService
 */
class Wol {
private:
    WiFiUDP Udp;
    WakeCallback onWakeUp;
    // Helper
    byte packetBuffer[102];

    bool isThermalUnsafe();

public:
    Wol();

    void setupWol(WakeCallback onWakeUpCallback);

    void handleWolLoop();
};

#endif //WOL_H
