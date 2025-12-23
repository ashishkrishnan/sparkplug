#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <WebServer.h>
#include <WiFiUdp.h>
#include "../logger/EventLogger.h"

typedef void (*WakeCallback)(String os, String strategy);

typedef void (*ShutDownCallback)();

class WebService {
private:
    WebServer server;
    WiFiUDP Udp;
    EventLogger logger;

    // Callbacks
    WakeCallback wakeCb;
    ShutDownCallback shutdownCb;

    // Helper
    byte packetBuffer[102];

    bool isThermalUnsafe();

    void checkWoL();

public:
    WebService();

    void setupWebAPI(WakeCallback onWake, ShutDownCallback onShutdown);

    void handleWebAPILoop();

    void logEvent(String msg);
};
#endif
