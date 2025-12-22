#ifndef WEBSERVICE_CPP
#define WEBSERVICE_CPP
#include "../../src/config/config.h"
#include "../../src/connectivity/connectivity.h"
#include "WebServer.h"
#include "WiFiUdp.h"

#include "webservice.h"

extern Connectivity network;

WebService::WebService() : server(HTTP_PORT) {
    // Constructor initializes server port
}

void WebService::logEvent(String msg) {
    String entry = "[" + String(millis()/1000) + "s] " + msg;

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

void WebService::handleHealth() {
    String json = "{";
    json += "\"status\":\"online\",";
    json += "\"ip\":\"" + network.getIpAddress() + "\",";
    json += "\"signal_dbm\":" + String(network.getWifiSignalStrength()) + ",";
    json += "\"temp_c\":" + String(network.getInternalTemp()) + ",";

    // Insert Logs
    json += "\"logs\":[";
    int count = wrapped ? MAX_LOGS : logIdx;
    int start = wrapped ? logIdx : 0;
    for(int i=0; i<count; i++) {
        int idx = (start + i) % MAX_LOGS;
        json += "\"" + logs[idx] + "\"";
        if(i < count-1) json += ",";
    }
    json += "]}";
    server.send(200, "application/json", json);
}

void WebService::handleWake() {
    String os = server.hasArg("os") ? server.arg("os") : OS_NAME_PRIMARY;

    if(isThermalUnsafe()) {
        server.send(503, "text/plain", "Thermal Lockout. Device too hot.");
        return;
    }

    logEvent("API Wake Req: " + os);
    server.send(200, "text/plain", "Sparking " + os);

    if(wakeCb) wakeCb(os);
}

void WebService::handleShutdown() {
    logEvent("API Shutdown Req. Checking Ping...");

    if(isThermalUnsafe()) {
        server.send(503, "text/plain", "Thermal Lockout. Device too hot.");
        return;
    }

    if(network.isTargetPCAlive(TARGET_PC_IP_ADDRESS)) {
        logEvent("Target ON. Shutting down.");
        server.send(200, "text/plain", "Executing Safe Shutdown");
        if(shutdownCb) shutdownCb();
    } else {
        logEvent("Target unreachable (OFF). Aborting.");
        server.send(409, "text/plain", "Target OFF. Aborted.");
    }
}

void WebService::checkWoL() {
    int size = Udp.parsePacket();
    if (size == 102) {
        Udp.read(packetBuffer, 102);
        if(packetBuffer[0] == 0xFF) {
            logEvent("WoL Recv from " + Udp.remoteIP().toString());
            if(!isThermalUnsafe() && wakeCb) wakeCb(OS_NAME_PRIMARY);
        }
    }
}

void WebService::setupWebAPI(WakeCallback onWake, ShutDownCallback onShutdown) {
    logEvent("WebService Starting");
    wakeCb = onWake;
    shutdownCb = onShutdown;

    server.on("/wake", HTTP_ANY, [this](){ handleWake(); });
    server.on("/shutdown", HTTP_ANY, [this](){ handleShutdown(); });
    server.on("/health", HTTP_GET, [this](){ handleHealth(); });

    server.begin();
    Udp.begin(WOL_PORT);

    logEvent("Service Ready! - All systems go!");
}

void WebService::handleWebAPILoop() {
    server.handleClient();
    checkWoL();
}

#endif
