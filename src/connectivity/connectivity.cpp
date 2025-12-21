#ifndef CONNECTIVITY_CPP
#define CONNECTIVITY_CPP

#include "../../src/connectivity/connectivity.h"
#include "../../src/config/config.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <ESPping.h>

class Connectivity : public IConnection {
    public:
        void setupConnectivity() override {
        WiFi.setHostname(HOSTNAME);
        WiFi.begin(WIFI_SSID, WIFI_PASS);

        while (WiFi.status() != WL_CONNECTED) delay(500);

        MDNS.begin(HOSTNAME);
        ArduinoOTA.setHostname(HOSTNAME);
        ArduinoOTA.begin();
    }

    void handleConnectivityLoop() override {
        ArduinoOTA.handle();
    }

    bool isTargetPCAlive(const char* targetIp) override {
        IPAddress addr;
        addr.fromString(targetIp);
        return Ping.ping(addr);
    }

    String getIpAddress() override {
        return WiFi.localIP().toString();
    }

    long getWifiSignalStrength() override {
        return WiFi.RSSI();
    }

    float getInternalTemp() override {
        return temperatureRead();
    }
};
#endif
