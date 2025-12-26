#include "webservice.h"
#include "../../src/config/config.h"
#include "../../src/connectivity/connectivity.h"
#include "../../src/boot/usbkeyboard.h"
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

void WebService::setupWebAPI(WakeCallback onWake, ShutDownCallback onShutdown) {
    logger.log("[Sparkplug] WebService Starting", network.getFormattedTime());
    wakeCb = onWake;
    shutdownCb = onShutdown;

    server.on("/health", HTTP_GET, [this]() {
        HealthRouter::handle(server, network, logger);
    });

    server.on("/wake", HTTP_ANY, [this, onWake]() {
        if (isThermalUnsafe()) {
            server.send(503, "text/plain", "Thermal Lockout");
            return;
        }
        WakeRouter::handle(server, bootSystem, network, logger, onWake);
    });

    server.on("/shutdown", HTTP_ANY, [this, onShutdown]() {
        if (isThermalUnsafe()) {
            server.send(503, "text/plain", "Thermal Lockout");
            return;
        }
        ShutdownRouter::handle(server, network, logger, onShutdown);
    });

    server.on("/debug/type", HTTP_GET, [this]() {
        DebugRouter::handleType(server, hwKb);
    });

    server.begin();
    Udp.begin(WOL_PORT);
    logger.log("[SparkPlug] Service Ready! - All systems go!", network.getFormattedTime());
}

void WebService::handleWebAPILoop() {
    server.handleClient();
    checkWoL();
}

void WebService::logEvent(String msg) {
    logger.log(msg, network.getFormattedTime());
}

bool WebService::isThermalUnsafe() {
    float currentTemp = network.getInternalTemp();
    if (currentTemp > MAX_TEMP_C) {
        logger.log("CRITICAL: Temp " + String(currentTemp) + "C exceeds limit!", network.getFormattedTime());
        return true;
    }
    return false;
}

void WebService::checkWoL() {
    int size = Udp.parsePacket();
    if (size == 102) {
        Udp.read(packetBuffer, 102);
        if (packetBuffer[0] == 0xFF) {
            logger.log("[WAKE] WoL Packet Received", network.getFormattedTime());
            if (!isThermalUnsafe() && wakeCb) {
                // Wake Ubuntu (Default) with Default Strategy
                wakeCb(OS_NAME_PRIMARY, DEFAULT_BOOT_STRATEGY);
            }
        }
    }
}
