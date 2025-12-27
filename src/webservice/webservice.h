#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <WebServer.h>

typedef void (*WakeCallback)(String os, String strategy);

typedef void (*ShutDownCallback)();

class WebService {
private:
    WebServer server;

    // Callbacks
    WakeCallback wakeCb;
    ShutDownCallback shutdownCb;

    bool isThermalUnsafe();

public:
    WebService();

    void setupWebAPI(WakeCallback onWake, ShutDownCallback onShutdown);

    void handleWebAPILoop();
};
#endif
