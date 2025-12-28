#ifndef WOL_H
#define WOL_H
#include <Arduino.h>
#include <WiFiUdp.h>

typedef void (*WakeCallback)(String, String);

/**
 * Handles Wake-on-lan functionality. In prior versions of sparkplug, it was present in WebService
 */
class Wol {
private:
    WiFiUDP Udp;
    WakeCallback onWakeUp;
    byte packetBuffer[102];
    uint8_t _systemMac[6];

public:
    Wol();

    void setupWol(WakeCallback onWakeUpCallback);

    void handleWolLoop();
};

#endif //WOL_H
