#include "src/boot/keyboard/usbkeyboard.h"
#include "src/power/relay.h"
#include "src/safety/safety.h"
#include "src/power/power.h"
#include "src/boot/boot.h"
#include "src/connectivity/connectivity.h"
#include "src/webservice/webservice.h"
#include "src/wakeonlan/wol.h"
#include "src/time/timeprovider.h"
#include "src/logger/EventLogger.h"
#include <Arduino.h>
#include "src/core/systemmanager.h"


#ifdef RUN_TESTS_ON_BOOT
  #include "src/tests/TestRunner.h"
#endif

Relay hwRelay;
USBKeyboard hwKb;

Connectivity *network;

Power power(&hwRelay);
Boot* bootSystem = nullptr;
Safety safety(network);
WebService web_service;
Wol* wol = nullptr;

void setup() {
    Serial.begin(115200);
    hwKb.init();
    delay(2000);

#ifdef RUN_TESTS_ON_BOOT
    Serial.println("--- STARTING TEST SUITE ---");
    runSuite();
    Serial.println("--- TESTS COMPLETE ---");
    while(1) delay(1000);
#else
    Log.log("[Sparkplug] Starting system");

    power.setup();
    network->setupWifi();
    time_provider.setup();
    Log.setTimeProvider([]() -> String {
        return time_provider.getFormattedTime();
    });
    network->setupHostName();
    safety.setup(network);

    bootSystem = new Boot(&hwKb);
    system_manager.setup(&power, bootSystem, &safety);

    wol = new Wol();
    wol->setupWol([](String os, String strategy) {
        system_manager.triggerWake(os, strategy, false, "WoL");
    });

    web_service.setupWebAPI();

    Log.log("[Sparkplug] Boot Complete. Ready.");
#endif
}

void loop() {
#ifndef RUN_TESTS_ON_BOOT
    network->handleConnectivityLoop();

    // Handle Wake-on-Lan requests
    wol->handleWolLoop();

    // Handle API requests
    web_service.handleWebAPILoop();
    system_manager.update();
#endif
}
