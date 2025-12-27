#ifndef HEALTH_ROUTER_H
#define HEALTH_ROUTER_H

#include <WebServer.h>
#include "../../connectivity/connectivity.h"
#include "../../logger/EventLogger.h"
#include "../../system/systeminfo.h"
#include "../../time/timeprovider.h"
#include "../../wakeonlan/WolVirtualAddress.h"

class HealthRouter {
public:
    static void handle(WebServer &server, Connectivity &network) {
        String refresh_interval = String(REFRESH_INTERVAL_FOR_HEALTH_API_IN_SECONDS);
        String refreshArg = server.hasArg("refresh") ? server.arg("refresh") : refresh_interval;
        if (refreshArg.toInt() < 1) refreshArg = refresh_interval;

        long rssi = network.getWifiSignalStrength();
        uint32_t freeRam = system_info.getFreeHeap();
        uint32_t totalRam = system_info.getTotalHeap();
        uint8_t mac[6];
        network.getMacBytes(mac);

        String json = "{";

        // System
        json += "\"system\": {";
        json += "\"status\": \"online\",";
        json += "\"uptime_s\": " + String(millis() / 1000) + ",";
        json += "\"uptime_str\": \"" + system_info.getUptime() + "\",";
        json += "\"server_time\": \"" + time_provider.getFormattedTime() + "\"";
        json += "},";

        // Hardware
        json += "\"hardware\": {";
        json += "\"chip\": \"" + system_info.getChipModel() + "\",";
        json += "\"free_ram_kb\": " + String(freeRam/1024) + ",";
        json += "\"total_ram_kb\": " + String(totalRam/1024) + ",";
        json += "\"temp_c\": " + String(system_info.getInternalTemp());
        json += "},";

        // Network
        json += "\"network\": {";
        json += "\"ip\": \"" + network.getIpAddress() + "\",";
        json += "\"mac\": \"" + network.getMacAddress() + "\",";
        json += "\"signal_dbm\": " + String(rssi) + ",";
        json += "\"virtual_mac\": {";
        json += "\"" + String(OS_NAME_PRIMARY) + "\": \"" +
                WolVirtualAddress::getVirtualMacString(mac, VIRTUAL_MAC_HEX_PRIMARY) + "\",";

        json += "\"" + String(OS_NAME_SECONDARY) + "\": \"" +
                WolVirtualAddress::getVirtualMacString(mac, VIRTUAL_MAC_HEX_SECONDARY) + "\"";

        json += "}";
        json += "},";

        json += "\"logs\": " + Log.getLogsAsJson();

        json += "}";

        server.sendHeader("Refresh", refreshArg);
        server.send(200, "application/json", json);
    }
};
#endif
