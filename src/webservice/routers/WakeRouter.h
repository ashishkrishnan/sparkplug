#ifndef WAKE_ROUTER_H
#define WAKE_ROUTER_H

#include <WebServer.h>
#include "../../boot/boot.h"
#include "../../connectivity/connectivity.h"
#include "../../logger/EventLogger.h"
#include "../webservice.h"

class WakeRouter {
public:
    static void handle(WebServer &server, Boot *bootSystem, Connectivity &network, EventLogger &logger,
                       WakeCallback cb) {
        if (bootSystem->isBusy()) {
            logger.log("[WAKE] Rejected - System Busy", network.getFormattedTime());
            server.send(429, "text/plain", "Busy: Sequence in progress");
            return;
        }

        bool force = server.hasArg("force") && server.arg("force") == "true";
        if (bootSystem->isCoolingDown() && !force) {
            long remaining = bootSystem->getCoolDownRemaining();
            String msg = "Safety Check: Cool-Down Active (" + String(remaining) + "s remaining). Use ?force=true.";
            logger.log("[WAKE] Rejected - Cool Down", network.getFormattedTime());
            server.send(429, "text/plain", msg);
            return;
        }

        if (network.isTargetPCAlive(TARGET_PC_IP_ADDRESS) && !force) {
            logger.log("[WAKE] Skipped - Target Online", network.getFormattedTime());
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

        logger.log("[WAKE] Start [" + os + "] Mode: " + strategy, network.getFormattedTime());
        server.send(200, "text/plain", "Wake Sequence Started for " + os);

        if (cb) cb(os, strategy);
    }
};
#endif
