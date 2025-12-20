#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
#include "local.h"

const char* HOSTNAME = "sparkplug";

// Wifi Configuration
const char* WIFI_SSID = LOCAL_WIFI_SSID;
const char* WIFI_PASS = LOCAL_WIFI_PASSWORD;

const int BOOT_DELAY_IN_MS = 9000;

const char* OS_NAME_PRIMARY = "ubuntu";
const char* OS_NAME_SECONDARY = "windows";

#endif