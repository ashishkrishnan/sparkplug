#ifndef CONNECTIVITY_CPP
#define CONNECTIVITY_CPP

#include "connectivity.h"
#include "../../src/config/config.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <ESPping.h>
#include <time.h>

void Connectivity::setupConnectivity() {
    WiFi.setHostname(HOSTNAME);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) delay(500);

    configTzTime(TIME_ZONE, NTP_SERVER);

    MDNS.begin(HOSTNAME);
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.begin();
}

void Connectivity::handleConnectivityLoop() {
    ArduinoOTA.handle();
}

bool Connectivity::isTargetPCAlive(const char* targetIp) {
    IPAddress addr;
    if(addr.fromString(targetIp)) {
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

float Connectivity::getInternalTemp() {
    return temperatureRead();
}

String Connectivity::getMacAddress() {
    return WiFi.macAddress();
}

String Connectivity::getChipModel() {
    return String(ESP.getChipModel()) + " Rev " + String(ESP.getChipRevision());
}

uint32_t Connectivity::getFreeHeap() {
    return ESP.getFreeHeap() / 1024;
}

uint32_t Connectivity::getTotalHeap() {
    return ESP.getHeapSize() / 1024;
}

String Connectivity::getFormattedTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        return "Syncing with NTP time ";
    }

    char timeStringBuff[30];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%d-%m-%Y %I:%M:%S %p", &timeinfo);
    return String(timeStringBuff);
}

String Connectivity::getUptime() {
    long totalSec = millis() / 1000;
    int days = totalSec / 86400;
    int hours = (totalSec % 86400) / 3600;
    int mins = (totalSec % 3600) / 60;
    int secs = totalSec % 60;

    String uptime = "";
    if(days > 0) uptime += String(days) + "d ";
    uptime += String(hours) + "h " + String(mins) + "m " + String(secs) + "s";
    return uptime;
}

#endif
