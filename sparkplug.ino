#include "src/boot/keyboard/usbkeyboard.h"
#include "src/power/relay.h"
#include "src/safety/health.h"
#include "src/power/power.h"
#include "src/boot/boot.h"
#include "src/safety/safety.h"
#include "src/connectivity/connectivity.h"
#include "src/webservice/webservice.h"
#include "src/wakeonlan/wol.h"
#include "src/time/timeprovider.h"
#include "src/logger/EventLogger.h"
#include <Arduino.h>


#ifdef RUN_TESTS_ON_BOOT
  #include "src/tests/TestRunner.h"
#endif

Relay hwRelay;
USBKeyboard hwKb;
Health hwHealth;

Connectivity network;

Power power(&hwRelay);
Boot* bootSystem = nullptr;
Safety safety(&hwHealth);
WebService web_service;
Wol* wol = nullptr;

#ifndef RUN_TESTS_ON_BOOT

void executeWake(String os, String strategy) {
    if(!safety.isSafeToOperate()) {
        Log.log("[CRITICAL] Wake Aborted. Thermal/Safety checked failed (>=" + String(MAX_TEMP_C) + ") degrees");
        return;
    }

    Log.log("[Wake] Pulsing Relay (Wake)...");
    power.triggerPulse();

    Log.log("[Wake] Waiting " + String(TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS/1000) + "s for BIOS...");

    bootSystem->startSequence(os, strategy);
}

void executeShutdown() {
    Log.log("[Shutdown] Shutdown Requested.");

    if(safety.isSafeShutdownAllowed()) {
        Log.log("[Shutdown] Target PC is ON. Pulsing Relay...");
        power.triggerPulse();
        bootSystem->startShutdown();
    } else {
        Log.log("[Shutdown] Target PC already OFF. Ignored.");
    }
}
#endif

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
    network.setupConnectivity();
    time_provider.setup();

    Log.setTimeProvider([]() -> String {
        return time_provider.getFormattedTime();
    });

    bootSystem = new Boot(&hwKb);
    wol = new Wol();

    // Pass the callbacks
    wol->setupWol(executeWake);
    web_service.setupWebAPI(executeWake, executeShutdown);

    Log.log("[Sparkplug] Boot Complete. Ready.");
#endif
}

void loop() {
#ifndef RUN_TESTS_ON_BOOT
    network.handleConnectivityLoop();

    // Handle Wake-on-Lan requests
    wol->handleWolLoop();

    // Handle API requests
    web_service.handleWebAPILoop();
    if (bootSystem) {
        bootSystem->update();
    }
#endif
}
