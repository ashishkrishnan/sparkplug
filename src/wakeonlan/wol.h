#ifndef WOL_H
#define WOL_H
#include <Arduino.h>
#include <WiFiUdp.h>

#include "../logger/EventLogger.h"

typedef void (*WakeCallback)(String os, String strategy);
typedef std::function<void(String)> Logger;

/**
 * Handles Wake-on-lan functionality. In prior versions of sparkplug, it was present in WebService
 */
class Wol {
private:
    WiFiUDP Udp;
    WakeCallback onWakeUp;
    // Helper
    byte packetBuffer[102];

    Logger _logger;

    bool isThermalUnsafe();

public:
    Wol(Logger logger);

    void setupWol(WakeCallback onWakeUpCallback);

    void handleWolLoop();
};

#endif //WOL_H
