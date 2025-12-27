#include "webservice.h"
#include "../../src/config/config.h"
#include "../../src/connectivity/connectivity.h"
#include "../boot/keyboard/usbkeyboard.h"
#include "../../src/boot/boot.h"

#include "Routers/HealthRouter.h"
#include "Routers/WakeRouter.h"
#include "Routers/ShutdownRouter.h"
#include "Routers/DebugRouter.h"
#include "../../src/system/systeminfo.h"

extern Connectivity network;
extern USBKeyboard hwKb;
extern Boot *bootSystem;

WebService::WebService() : server(HTTP_PORT) {
}

void WebService::setupWebAPI(WakeCallback onWake, ShutDownCallback onShutdown) {
    Log.log("[Sparkplug] WebService Starting");
    wakeCb = onWake;
    shutdownCb = onShutdown;

    server.on("/health", HTTP_GET, [this]() {
        HealthRouter::handle(server, network);
    });

    server.on("/wake", HTTP_ANY, [this, onWake]() {
        if (isThermalUnsafe()) {
            server.send(503, "text/plain", "Thermal Lockout");
            return;
        }
        WakeRouter::handle(server, bootSystem, network, onWake);
    });

    server.on("/shutdown", HTTP_ANY, [this, onShutdown]() {
        if (isThermalUnsafe()) {
            server.send(503, "text/plain", "Thermal Lockout");
            return;
        }
        ShutdownRouter::handle(server, network, onShutdown);
    });

    server.on("/debug/type", HTTP_GET, [this]() {
        DebugRouter::handleType(server, hwKb);
    });

    server.begin();
    Log.log("[Sparkplug] Service Ready! - All systems go!");
}

void WebService::handleWebAPILoop() {
    server.handleClient();
}

void WebService::logEvent(String msg) {
    Log.log(msg);
}

bool WebService::isThermalUnsafe() {
    float currentTemp = system_info.getInternalTemp();
    if (currentTemp > MAX_TEMP_C) {
        Log.log("CRITICAL: Temp " + String(currentTemp) + "C exceeds limit!");
        return true;
    }
    return false;
}
