#ifndef TESTRUNNER_H
#define TESTRUNNER_H
#include <Arduino.h>

#include "boot/boot.h"
#include "mocks/MockHealth.h"
#include "mocks/MockKeyboard.h"
#include "safety/safety.h"

void assertTest(String name, bool condition, String error) {
    Serial.println("[Test] " + name + ": ");
    Serial.println(condition ? "PASS" : "FAIL");
    Serial.println(error);
}

void runSuite() {
    Serial.println("[Test] " + String("Running test suite"));

    // Test 1
    MockKeyboard mk;
    Boot bl(&mk);

    bl.selectOS("windows");
    bool ok = mk.log.indexOf("KEY:D9") != -1 && mk.log.indexOf("KEY:B0") != -1;
    assertTest("Windows Sequence", ok, mk.log);

    // Test 2
    MockHealth mh;
    mh.fakeTemp = 90.0;
    Safety sy(&mh);

    assertTest("Overheat Block", !sy.isSafeToOperate(), "Allowed > 85C");

    // Test 3
    mh.fakeTemp = 40.0;
    mh.fakePing = false;
    assertTest("Offline Shutdown Block", !sy.isSafeShutdownAllowed(), "Allowed offline shutdown");

    Serial.print("End of test suite");
}

#endif //TESTRUNNER_H
