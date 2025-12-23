#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <Arduino.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include "../../src/config/config.h"

typedef void (*WakeCallback)(String os, String strategy);
typedef void (*ShutDownCallback)();

class WebService {
private:
    WebServer server;
    WiFiUDP Udp;
    uint8_t packetBuffer[102]{};

    // Callbacks
    WakeCallback wakeCb{};
    ShutDownCallback shutdownCb{};

    String logs[MAX_LOGS];
    int logIdx = 0;
    bool wrapped = false;

    // Internal Helpers
    bool isThermalUnsafe();
    void handleHealth();
    void handleWake();
    void handleShutdown();
    void checkWoL();
    void debugTyping();

public:
    // Constructor
    WebService();

    // Public API
    void setupWebAPI(WakeCallback onWake, ShutDownCallback onShutdown);
    void handleWebAPILoop();
    void logEvent(String msg);
};
#endif // WEBSERVICE_H