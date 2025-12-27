#ifndef CONNECTIVITY_CPP
#define CONNECTIVITY_CPP

#include "connectivity.h"
#include "../../src/config/config.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <ESPping.h>

void Connectivity::setupWifi() {
    WiFi.setHostname(HOSTNAME);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) delay(500);
}

void Connectivity::setupHostName() {
    MDNS.begin(HOSTNAME);
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.begin();
}

void Connectivity::handleConnectivityLoop() {
    ArduinoOTA.handle();
}

bool Connectivity::isTargetPCAlive(const char *targetIp) {
    IPAddress addr;
    if (addr.fromString(targetIp)) {
        return Ping.ping(addr);
    }
    return false;
}

String Connectivity::getIpAddress() {
    return WiFi.localIP().toString();
}

long Connectivity::getWifiSignalStrength() {
    return WiFi.RSSI();
}

String Connectivity::getMacAddress() {
    return WiFi.macAddress();
}

#endif
