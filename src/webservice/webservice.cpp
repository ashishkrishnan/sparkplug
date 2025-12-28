#include "webservice.h"
#include "../../src/config/config.h"
#include "../../src/connectivity/connectivity.h"
#include "../boot/keyboard/usbkeyboard.h"
#include "../../src/boot/boot.h"

#include "Routers/HealthRouter.h"
#include "Routers/WakeRouter.h"
#include "Routers/ShutdownRouter.h"
#include "Routers/DebugRouter.h"

extern Connectivity network;
extern USBKeyboard hwKb;
extern Boot *bootSystem;

WebService::WebService() : server(HTTP_PORT) {
}

void WebService::setupWebAPI() {
    Log.log("[Webservice] WebService Starting");

    server.on("/health", HTTP_GET, [this]() {
        HealthRouter::handle(server, network);
    });

    server.on("/wake", HTTP_ANY, [this]() {
        WakeRouter::handle(server);
    });

    server.on("/shutdown", HTTP_ANY, [this]() {
        ShutdownRouter::handle(server);
    });

    server.on("/debug/type", HTTP_GET, [this]() {
        DebugRouter::handleType(server, hwKb);
    });

    server.begin();
    Log.log("[WebService] Service Ready!");
}

void WebService::handleWebAPILoop() {
    server.handleClient();
}

