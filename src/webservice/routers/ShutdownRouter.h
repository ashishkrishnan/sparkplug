#ifndef SHUTDOWN_ROUTER_H
#define SHUTDOWN_ROUTER_H

#include <WebServer.h>
#include "../../core/systemmanager.h"

class ShutdownRouter {
public:
    static void handle(WebServer &server) {
        bool force = (server.arg("force") == "true");
        CommandResult result = system_manager.triggerShutdown(force, "API");
        switch (result) {
            case CommandResult::SUCCESS:
                server.send(200, "text/plain", "Shutdown Sequence Started");
                break;

            case CommandResult::ALREADY_OFFLINE:
                server.send(409, "text/plain", "Target PC is already Offline. Use ?force=true");
                break;

            case CommandResult::BUSY:
                server.send(429, "text/plain", "System Busy: Sequence in progress");
                break;

            case CommandResult::THERMAL_UNSAFE:
                server.send(503, "text/plain", "Critical: System Unsafe (Thermal Limit)");
                break;
        }
    }
};
#endif
