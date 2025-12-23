#ifndef HEALTH_ROUTER_H
#define HEALTH_ROUTER_H

#include <WebServer.h>
#include "../../connectivity/connectivity.h"
#include "../../logger/EventLogger.h"

class HealthRouter {
public:
    static void handle(WebServer &server, Connectivity &network, EventLogger &logger) {
        String refresh_interval = String(REFRESH_INTERVAL_FOR_HEALTH_API_IN_SECONDS);
        String refreshArg = server.hasArg("refresh") ? server.arg("refresh") : refresh_interval;
        if (refreshArg.toInt() < 1) refreshArg = refresh_interval;

        long rssi = network.getWifiSignalStrength();
        uint32_t freeRam = network.getFreeHeap();
        uint32_t totalRam = network.getTotalHeap();

        String json = "{";

        // System
        json += "\"system\": {";
        json += "\"status\": \"online\",";
        json += "\"uptime_s\": " + String(millis() / 1000) + ",";
        json += "\"uptime_str\": \"" + network.getUptime() + "\",";
        json += "\"server_time\": \"" + network.getFormattedTime() + "\"";
        json += "},";

        // Hardware
        json += "\"hardware\": {";
        json += "\"chip\": \"" + network.getChipModel() + "\",";
        json += "\"free_ram_kb\": " + String(freeRam) + ",";
        json += "\"total_ram_kb\": " + String(totalRam) + ",";
        json += "\"temp_c\": " + String(network.getInternalTemp());
        json += "},";

        // Network
        json += "\"network\": {";
        json += "\"ip\": \"" + network.getIpAddress() + "\",";
        json += "\"mac\": \"" + network.getMacAddress() + "\",";
        json += "\"signal_dbm\": " + String(rssi);
        json += "},";

        json += "\"logs\": " + logger.getLogsAsJson();

        json += "}";

        server.sendHeader("Refresh", refreshArg);
        server.send(200, "application/json", json);
    }
};
#endif
