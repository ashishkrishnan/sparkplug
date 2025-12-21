#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
#include "local.h"

const char* HOSTNAME = "sparkplug";

// Wifi Configuration
const char* WIFI_SSID = LOCAL_WIFI_SSID;
const char* WIFI_PASS = LOCAL_WIFI_PASSWORD;

constexpr int BOOT_DELAY_IN_MS = LOCAL_BOOT_DELAY_IN_MS;

const char* OS_NAME_PRIMARY = LOCAL_OS_NAME_PRIMARY;
const char* OS_NAME_SECONDARY = LOCAL_OS_NAME_SECONDARY;

constexpr int GRUB_SECONDARY_OS_POSITION = LOCAL_GRUB_SECONDARY_OS_POSITION;

const float MAX_TEMP_C = 85.0;

const char* TARGET_PC_IP_ADDRESS = LOCAL_TARGET_PC_IP_ADDRESS;

#endif
