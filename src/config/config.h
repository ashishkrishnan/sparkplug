#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
#include "local.h"

// Change your host name
static const char* HOSTNAME = "sparkplug";

// Wifi Configuration - Local
static const char* WIFI_SSID = LOCAL_WIFI_SSID;
static const char* WIFI_PASS = LOCAL_WIFI_PASSWORD;

// Target PC Configuration - Local
static const char* OS_NAME_PRIMARY = LOCAL_OS_NAME_PRIMARY;
static const char* OS_NAME_SECONDARY = LOCAL_OS_NAME_SECONDARY;
static constexpr int BOOT_DELAY_IN_MS = LOCAL_BOOT_DELAY_IN_MS;
static constexpr int GRUB_SECONDARY_OS_POSITION = LOCAL_GRUB_SECONDARY_OS_POSITION;
static const char* TARGET_PC_IP_ADDRESS = LOCAL_TARGET_PC_IP_ADDRESS;

// ESP Configuration
static constexpr uint8_t PIN_RELAY = 4;
static const float MAX_TEMP_C = 85.0;

// Web server
static constexpr int HTTP_PORT = 80;
static constexpr int MAX_LOGS = 200;
static constexpr int WOL_PORT = 9;

static const char* NTP_SERVER    = "pool.ntp.org";
static const char* TIME_ZONE     = LOCAL_TIME_ZONE; // e.g "IST-5:30" which is Asia/Kolkata TODO write/find a library to convert standard timezones into this format.

static constexpr int REFRESH_INTERVAL_FOR_HEALTH_API_IN_SECONDS = 30;

#endif
