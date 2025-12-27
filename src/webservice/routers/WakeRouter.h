#ifndef WAKE_ROUTER_H
#define WAKE_ROUTER_H

#include <WebServer.h>
#include "../../boot/boot.h"
#include "../../connectivity/connectivity.h"
#include "../../logger/EventLogger.h"
#include "../webservice.h"

class WakeRouter {
public:
    static void handle(WebServer &server, Boot *bootSystem, Connectivity &network, WakeCallback cb) {
        if (bootSystem->isBusy()) {
            Log.log("[Wake] Rejected - System Busy", time_provider.getFormattedTime());
            server.send(429, "text/plain", "Busy: Sequence in progress");
            return;
        }

        bool force = server.hasArg("force") && server.arg("force") == "true";
        if (bootSystem->isCoolingDown() && !force) {
            long remaining = bootSystem->getCoolDownRemaining();
            String msg = "Safety Check: Cool-Down Active (" + String(remaining) + "s remaining). Use ?force=true.";
            Log.log("[Wake] Rejected - Cool Down", time_provider.getFormattedTime());
            server.send(429, "text/plain", msg);
            return;
        }

        if (network.isTargetPCAlive(TARGET_PC_IP_ADDRESS) && !force) {
            Log.log("[Wake] Skipped - Target PC Online", time_provider.getFormattedTime());
            server.send(409, "text/plain", "Target PC is already Online");
            return;
        }

        String os = server.hasArg("os") ? server.arg("os") : OS_NAME_PRIMARY;

        if (os == "primary" || os == OS_NAME_PRIMARY || os == "first" || os == "default") {
            os = OS_NAME_PRIMARY;
        } else if (os == "secondary" || os == OS_NAME_SECONDARY || os == "second") {
            os = OS_NAME_SECONDARY;
        }

        String strategy = server.hasArg("strategy") ? server.arg("strategy") : DEFAULT_BOOT_STRATEGY;

        Log.log("[Wake] Start [" + os + "] Mode: " + strategy, time_provider.getFormattedTime());
        server.send(200, "text/plain", "Wake Sequence Started for " + os);

        if (cb) cb(os, strategy);
    }
};
#endif
