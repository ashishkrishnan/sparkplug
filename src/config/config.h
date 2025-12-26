#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
#include "wifi.h"

// --------- User settings --------

// Configure your host name
static const char* HOSTNAME = "sparkplug";

// Set your locale
static const char* TIME_ZONE     = "IST-5:30"; // Asia/Kolkata TODO write/find a library to convert standard timezones into this format.

// Set your Wifi
static const char* WIFI_SSID = LOCAL_WIFI_SSID;
static const char* WIFI_PASS = SECRET_WIFI_PASSWORD;

// Your target PC Config
static const char* OS_NAME_PRIMARY = "ubuntu";
static const char* OS_NAME_SECONDARY = "windows";
static constexpr int TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS = 19000;
static constexpr int GRUB_SECONDARY_OS_POSITION = 5;
static const char* TARGET_PC_IP_ADDRESS = "192.168.0.10";

// ---- Advanced Configuration ----

static constexpr int REFRESH_INTERVAL_FOR_HEALTH_API_IN_SECONDS = 30;
constexpr unsigned long COOLDOWN_PERIOD_IN_SECONDS = 30;

static const char* DEFAULT_BOOT_STRATEGY = "standard";

// ESP Configuration
static constexpr uint8_t PIN_RELAY = 4;
static const float MAX_TEMP_C = 85.0;

// Web server
static constexpr int HTTP_PORT = 80;
static constexpr int MAX_LOGS = 200;
static constexpr int WOL_PORT = 9;
static const char* NTP_SERVER    = "pool.ntp.org";

#endif
