#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <Arduino.h>

class SystemInfo {
public:
    String getUptime() {
        unsigned long now = millis() / 1000;
        int days = now / 86400;
        int hours = (now % 86400) / 3600;
        int minutes = (now % 3600) / 60;
        int seconds = now % 60;

        char buffer[30];
        snprintf(buffer, sizeof(buffer), "%dd %02dh %02dm %02ds", days, hours, minutes, seconds);
        return String(buffer);
    }

    long getUptimeSeconds() {
        return millis() / 1000;
    }

    uint32_t getFreeHeap() {
        return ESP.getFreeHeap();
    }
    uint32_t getTotalHeap() {
        return ESP.getHeapSize();
    }

    float getInternalTemp() {
        return temperatureRead();
    }

    String getChipModel() {
        return String(ESP.getChipModel());
    }
};

extern SystemInfo system_info;

#endif