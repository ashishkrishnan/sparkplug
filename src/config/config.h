#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
#include "local.h"

static const char* HOSTNAME = "sparkplug";

// Wifi Configuration
static const char* WIFI_SSID = LOCAL_WIFI_SSID;
static const char* WIFI_PASS = LOCAL_WIFI_PASSWORD;

static constexpr int BOOT_DELAY_IN_MS = LOCAL_BOOT_DELAY_IN_MS;

static const char* OS_NAME_PRIMARY = LOCAL_OS_NAME_PRIMARY;
static const char* OS_NAME_SECONDARY = LOCAL_OS_NAME_SECONDARY;

static constexpr int GRUB_SECONDARY_OS_POSITION = LOCAL_GRUB_SECONDARY_OS_POSITION;

static const float MAX_TEMP_C = 85.0;

static const char* TARGET_PC_IP_ADDRESS = LOCAL_TARGET_PC_IP_ADDRESS;

static constexpr uint8_t PIN_RELAY = 4;


#endif
