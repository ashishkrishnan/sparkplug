#ifndef SHUTDOWN_CONTROLLER_H
#define SHUTDOWN_CONTROLLER_H

#include <WebServer.h>
#include "../../connectivity/connectivity.h"
#include "../../logger/EventLogger.h"
#include "../webservice.h"

class ShutdownRouter {
public:
    static void handle(WebServer &server, Connectivity &network, ShutDownCallback cb) {
        Log.log("[Shutdown] Request received. Sending Ping to Target PC...");

        if (network.isTargetPCAlive(TARGET_PC_IP_ADDRESS)) {
            Log.log("[Shutdown] Target PC is ON. Executing...");
            server.send(200, "text/plain", "Executing Safe Shutdown");
            if (cb) cb();
        } else {
            Log.log("[Shutdown] Target PC is unreachable (OFF). Aborting.");
            server.send(409, "text/plain", "Target PC is already OFF.");
        }
    }
};
#endif
