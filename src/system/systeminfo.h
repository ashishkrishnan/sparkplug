#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <Arduino.h>

class SystemInfo {
public:
    String getUptime() {
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
        return String(ESP.getChipModel()) + " Rev " + String(ESP.getChipRevision());
    }
};

extern SystemInfo system_info;

#endif
