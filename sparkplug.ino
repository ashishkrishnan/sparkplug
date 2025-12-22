#include "src/boot/usbkeyboard.h"
#include "src/power/relay.h"
#include "src/safety/health.h"
#include "src/power/power.h"
#include "src/boot/boot.h"
#include "src/safety/safety.h"
#include "src/connectivity/connectivity.cpp"
#include "src/webapi/webservice.cpp"
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
WebService webApi;

#ifndef RUN_TESTS_ON_BOOT
void executeWake(String os) {
    if(!safety.isSafeToOperate()) return;
    power.triggerPulse();
    boot.selectOS(os);
}

void executeShutdown() {
    if(safety.isSafeShutdownAllowed()) power.triggerPulse();
}
#endif

void setup() {
    Serial.begin(115200);
    delay(2000);

#ifdef RUN_TESTS_ON_BOOT
    Serial.println("--- STARTING TEST SUITE ---");
    runSuite(); // Run tests once
    Serial.println("--- TESTS COMPLETE ---");
    while(1) delay(1000); // Halt system
#else
    // Production Setup
    power.setup();
    network.setupConnectivity();
    webApi.setupWebAPI(executeWake, executeShutdown);
#endif
}

void loop() {
#ifndef RUN_TESTS_ON_BOOT
    // Production Loop
    network.handleConnectivityLoop();
    webApi.handleWebAPILoop();
#endif
}
