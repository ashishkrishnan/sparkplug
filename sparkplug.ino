#include "src/boot/usbkeyboard.h"
#include "src/power/relay.h"
#include "src/safety/health.h"
#include "src/power/power.h"
#include "src/boot/boot.h"
#include "src/safety/safety.h"
#include "src/connectivity/connectivity.h"
#include "src/webservice//webservice.h"
#include <Arduino.h>

#ifdef RUN_TESTS_ON_BOOT
  #include "src/tests/TestRunner.h"
#endif

Relay hwRelay;
USBKeyboard hwKb;
Health hwHealth;

Connectivity network;

Power power(&hwRelay);
Boot boot(&hwKb);
Safety safety(&hwHealth);
WebService web_service;

#ifndef RUN_TESTS_ON_BOOT

void executeWake(String os) {
    if(!safety.isSafeToOperate()) {
        web_service.logEvent(
            "[Critical] Wake Aborted. Thermal/Safety checked failed (>=" + String(MAX_TEMP_C) + ") degrees");
        return;
    }

    web_service.logEvent("[Wake] Pulsing Relay (Wake)...");
    power.triggerPulse();

    web_service.logEvent("[Wake] Waiting " + String(BOOT_DELAY_IN_MS/1000) + "s for BIOS...");

    boot.selectOS(os);

    web_service.logEvent("[Wake] Success. Selected " + os);
}

void executeShutdown() {
    web_service.logEvent("[Shutdown] Shutdown Requested.");

    if(safety.isSafeShutdownAllowed()) {
        web_service.logEvent("[Shutdown] Target is ON. Pulsing Relay...");
        power.triggerPulse();
    } else {
        web_service.logEvent("[Shutdown] Target already OFF. Ignored.");
    }
}
#endif

void setup() {
    Serial.begin(115200);
    delay(2000);

#ifdef RUN_TESTS_ON_BOOT
    Serial.println("--- STARTING TEST SUITE ---");
    runSuite();
    Serial.println("--- TESTS COMPLETE ---");
    while(1) delay(1000);
#else
    // Production Setup
    web_service.logEvent("[Sparkplug] Starting system");

    power.setup();
    network.setupConnectivity();

    // Pass the callbacks
    web_service.setupWebAPI(executeWake, executeShutdown);

    web_service.logEvent("[Sparkplug] Boot Complete. Ready.");
#endif
}

void loop() {
#ifndef RUN_TESTS_ON_BOOT
    network.handleConnectivityLoop();

    // Handle API requests
    web_service.handleWebAPILoop();
#endif
}