#include "connectivity.h"
#include "config/config.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

void setupConnectivity() {
    WiFi.setHostname(HOSTNAME);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) delay(500);

    MDNS.begin(HOSTNAME);
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.begin();
}
