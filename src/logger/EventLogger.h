#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <Arduino.h>
#include <functional>
#include "../config/config.h"

class EventLogger {
private:
    String logs[MAX_LOGS];
    int logIdx = 0;
    bool wrapped = false;

    std::function<String()> timeProvider = []() { return String("NO SYNC"); };

public:
    void setTimeProvider(std::function<String()> provider) {
        timeProvider = provider;
    }

    void log(String msg, String timeStr) {
        String entry = "[" + timeStr + "] " + msg;

        logs[logIdx] = entry;
        logIdx++;
        if (logIdx >= MAX_LOGS) {
            logIdx = 0;
            wrapped = true;
        }
        Serial.println(entry);
    }

    void log(String message) {
        String timeString = timeProvider();
        String entry = "[" + timeString + "] " + message;

        logs[logIdx] = entry;
        logIdx++;
        if (logIdx >= MAX_LOGS) {
            logIdx = 0;
            wrapped = true;
        }

        Serial.println(entry);
    }

    String getLogsAsJson() {
        String json = "[";
        int count = wrapped ? MAX_LOGS : logIdx;
        int start = wrapped ? logIdx : 0;

        for (int i = 0; i < count; i++) {
            int idx = (start + i) % MAX_LOGS;
            String cleanLog = logs[idx];
            cleanLog.replace("\"", "'"); // Sanitize JSON

            json += "\"" + cleanLog + "\"";
            if (i < count - 1) json += ",";
        }
        json += "]";
        return json;
    }
};

extern EventLogger Log;
#endif
