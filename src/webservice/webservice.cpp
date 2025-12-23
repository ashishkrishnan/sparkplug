#ifndef WEBSERVICE_CPP
#define WEBSERVICE_CPP
#include "../../src/config/config.h"
#include "../../src/connectivity/connectivity.h"
#include "WebServer.h"
#include "WiFiUdp.h"
#include "../../src/boot/usbkeyboard.h"
#include "webservice.h"

extern Connectivity network;
extern USBKeyboard hwKb;

WebService::WebService() : server(HTTP_PORT) {
    // Constructor initializes server port
}

void WebService::handleWebAPILoop() {
    server.handleClient();
    checkWoL();
}

void WebService::setupWebAPI(WakeCallback onWake, ShutDownCallback onShutdown) {
    logEvent("[Sparkplug] WebService Starting");
    wakeCb = onWake;
    shutdownCb = onShutdown;

    server.on("/wake", HTTP_ANY, [this](){ handleWake(); });
    server.on("/shutdown", HTTP_ANY, [this](){ handleShutdown(); });
    server.on("/health", HTTP_GET, [this](){ handleHealth(); });

    server.on("/debug/type", HTTP_GET, [this]() { debugTyping(); });

    server.begin();
    Udp.begin(WOL_PORT);

    logEvent("[SparkPlug] Service Ready! - All systems go!");
}

void WebService::logEvent(String msg) {
    String entry = "[" + network.getFormattedTime() + "] " + msg;

    logs[logIdx] = entry;
    logIdx++;
    if (logIdx >= MAX_LOGS) { logIdx = 0; wrapped = true; }
    Serial.println(entry);
}

bool WebService::isThermalUnsafe() {
    float currentTemp = network.getInternalTemp();

    if (currentTemp > MAX_TEMP_C) {
        logEvent("CRITICAL: Temp " + String(currentTemp) + "C exceeds limit!");
        return true;
    }
    return false;
}
/**
 * Route: /health
 *
 * TODO(ak) move this to a separate class.
 */
void WebService::handleHealth() {
    String refresh_interval = String(REFRESH_INTERVAL_FOR_HEALTH_API_IN_SECONDS);
    String refreshArg = server.hasArg("refresh") ? server.arg("refresh") : refresh_interval;
    // Prevent crazy fast refreshes (min 1 sec)
    if (refreshArg.toInt() < 1) refreshArg = refresh_interval;

    long rssi = network.getWifiSignalStrength();
    uint32_t freeRam = network.getFreeHeap();
    uint32_t totalRam = network.getTotalHeap();

    String json = "{";

    json += "\"system\": {";
    json += "\"status\": \"online\",";
    json += "\"uptime_s\": " + String(millis() / 1000) + ","; // Raw seconds is better for UI math
    json += "\"uptime_str\": \"" + network.getUptime() + "\",";
    json += "\"server_time\": \"" + network.getFormattedTime() + "\"";
    json += "},";

    json += "\"hardware\": {";
    json += "\"chip\": \"" + network.getChipModel() + "\",";
    json += "\"free_ram_kb\": " + String(freeRam) + ",";
    json += "\"total_ram_kb\": " + String(totalRam) + ",";
    json += "\"temp_c\": " + String(network.getInternalTemp());
    json += "},";

    json += "\"network\": {";
    json += "\"ip\": \"" + network.getIpAddress() + "\",";
    json += "\"mac\": \"" + network.getMacAddress() + "\",";
    json += "\"signal_dbm\": " + String(rssi);
    json += "},";

    json += "\"logs\": [";
    int count = wrapped ? MAX_LOGS : logIdx;
    int start = wrapped ? logIdx : 0;
    for(int i=0; i<count; i++) {
        int idx = (start + i) % MAX_LOGS;
        String cleanLog = logs[idx];
        cleanLog.replace("\"", "'");

        json += "\"" + cleanLog + "\"";
        if(i < count-1) json += ",";
    }
    json += "]";
    json += "}";

    server.sendHeader("Refresh", refreshArg);
    server.send(200, "application/json", json);
}

/**
 * Route /wake
 * Queries
 * os: operating system of your choice from primary/secondary
 * strategy: standard / aggressive - a technique that works for some motherboards to allow HID during POST.
 *
 * example: /wake?os=ubuntu&strategy=standard or /wake?os=ubuntu&strategy=aggressive
 * * TODO(ak) move this to a separate class.
 */
void WebService::handleWake() {
    if(isThermalUnsafe()) {
        server.send(503, "text/plain", "Thermal Lockout. Sparkplug too hot.");
        return;
    }

    const String os = server.hasArg("os") ? server.arg("os") : OS_NAME_PRIMARY;
    const String strategy = server.hasArg("strategy") ? server.arg("strategy") : DEFAULT_BOOT_STRATEGY;

    logEvent("API Wake Req: " + os + " [Mode: " + strategy + "]");

    server.send(200, "text/plain", "OK: Sparking" + os + "with " + strategy + " strategy..");

    if(wakeCb) wakeCb(os, strategy);
}

/**
 * Route: /shutdown
 *
 * TODO(ak) move this to a separate class.
 */
void WebService::handleShutdown() {
    logEvent("[Shutdown] Shutdown Request. Checking Ping...");
    if(isThermalUnsafe()) {
        server.send(503, "text/plain", "Thermal Lockout. Sparkplug too hot.");
        return;
    }

    if(network.isTargetPCAlive(TARGET_PC_IP_ADDRESS)) {
        logEvent("[Shutdown] Target ON. Shutting down.");
        server.send(200, "text/plain", "Executing Safe Shutdown");
        if(shutdownCb) shutdownCb();
    } else {
        logEvent("[Shutdown] Target unreachable (OFF). Aborting.");
        server.send(409, "text/plain", "Target OFF. Aborted.");
    }
}

/**
 * Standard UDP protocol for WakeOnLan
 */
void WebService::checkWoL() {
    int size = Udp.parsePacket();
    if (size == 102) {
        Udp.read(packetBuffer, 102);
        if(packetBuffer[0] == 0xFF) {
            logEvent("[Wake] Wake up on Lan (WoL) received from " + Udp.remoteIP().toString());
            if(!isThermalUnsafe() && wakeCb) wakeCb(OS_NAME_PRIMARY, DEFAULT_BOOT_STRATEGY);
        }
    }
}

/**
 * Route: /debug/type?key=enter
 * Comes in handy to check if the HID is working as expected.
 * TODO(ak) move this to a separate class.
 */
void WebService::debugTyping() {
    if (!server.hasArg("key")) {
        server.send(400, "text/plain", "Missing 'key'");
        return;
    }

    String keyName = server.arg("key");
    keyName.toLowerCase();
    uint8_t keyCode = 0;

    if (keyName == "shift") keyCode = KEY_SHIFT;
    else if (keyName == "enter") keyCode = KEY_ENTER;
    else if (keyName == "esc") keyCode = KEY_ESCAPE;
    else if (keyName == "backspace") keyCode = KEY_BACK;
    else if (keyName == "win") keyCode = KEY_GUI;
    else if (keyName == "up") keyCode = KEY_UP;
    else if (keyName == "down") keyCode = KEY_DOWN;
    else if (keyName == "left") keyCode = KEY_LEFT;
    else if (keyName == "right") keyCode = KEY_RIGHT;
    else if (keyName.length() == 1) keyCode = keyName.charAt(0);

    if (keyCode != 0) {
        hwKb.pressKey(keyCode);
        delay(100);
        hwKb.releaseAll();
        server.send(200, "text/plain", "Typed: " + keyName);
    } else {
        server.send(400, "text/plain", "Unknown Key");
    }
}

#endif
