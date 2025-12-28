#ifndef SHUTDOWN_ROUTER_H
#define SHUTDOWN_ROUTER_H

#include <WebServer.h>
#include "../../core/systemmanager.h"

class ShutdownRouter {
public:
    static void handle(WebServer &server) {
        CommandResult result = system_manager.triggerShutdown("Web");
        switch (result) {
            case CommandResult::SUCCESS:
                server.send(200, "text/plain", "Shutdown Sequence Started");
                break;

            case CommandResult::ALREADY_OFFLINE:
                server.send(409, "text/plain", "Target PC is already Offline.");
                break;

            case CommandResult::BUSY:
                server.send(429, "text/plain", "System Busy: Sequence in progress");
                break;

            case CommandResult::THERMAL_UNSAFE:
                server.send(503, "text/plain", "Critical: System Unsafe (Thermal Limit)");
                break;
            case CommandResult::COOLING_DOWN: {
                long remaining = system_manager.getCoolDownRemaining();
                String msg = "Safety Check: Cool-Down Active (" + String(remaining) +
                             "s remaining).";
                server.send(429, "text/plain", msg);
                break;
            }
        }
    }
};
#endif
