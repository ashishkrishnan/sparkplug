#ifndef WAKE_ROUTER_H
#define WAKE_ROUTER_H

#include <WebServer.h>
#include "../../core/systemmanager.h"

class WakeRouter {
public:
    static void handle(WebServer &server) {
        bool force = (server.arg("force") == "true");

        String os = server.hasArg("os") ? server.arg("os") : OS_NAME_PRIMARY;
        if (os == "primary" || os == OS_NAME_PRIMARY || os == "first" || os == "default") {
            os = OS_NAME_PRIMARY;
        } else if (os == "secondary" || os == OS_NAME_SECONDARY || os == "second") {
            os = OS_NAME_SECONDARY;
        }

        String strategy = server.hasArg("strategy") ? server.arg("strategy") : DEFAULT_BOOT_STRATEGY;
        CommandResult result = system_manager.triggerWake(os, strategy, force, "Web");

        switch (result) {
            case CommandResult::SUCCESS:
                server.send(200, "text/plain", "Wake Sequence Started for " + os + " using " + strategy + " strategy");
                break;

            case CommandResult::BUSY:
                server.send(429, "text/plain", "System Busy: Sequence in progress");
                break;

            case CommandResult::COOLING_DOWN: {
                long remaining = system_manager.getCoolDownRemaining();
                String msg = "Safety Check: Cool-Down Active (" + String(remaining) +
                             "s remaining). Use ?force=true to override.";
                server.send(429, "text/plain", msg);
                break;
            }

            case CommandResult::ALREADY_ONLINE:
                server.send(409, "text/plain", "Target PC is already Online");
                break;

            case CommandResult::THERMAL_UNSAFE:
                server.send(503, "text/plain", "Critical: System Unsafe (Thermal Limit Exceeded)");
                break;
        }
    }
};
#endif
