#ifndef SHUTDOWN_CONTROLLER_H
#define SHUTDOWN_CONTROLLER_H

#include <WebServer.h>
#include "../../connectivity/connectivity.h"
#include "../../logger/EventLogger.h"
#include "../webservice.h"

class ShutdownRouter {
public:
    static void handle(WebServer &server, Connectivity &network, EventLogger &logger, ShutDownCallback cb) {
        logger.log("[Shutdown] Request received. Checking Ping...", network.getFormattedTime());

        if (network.isTargetPCAlive(TARGET_PC_IP_ADDRESS)) {
            logger.log("[Shutdown] Target ON. Executing...", network.getFormattedTime());
            server.send(200, "text/plain", "Executing Safe Shutdown");
            if (cb) cb();
        } else {
            logger.log("[Shutdown] Target unreachable (OFF). Aborting.", network.getFormattedTime());
            server.send(409, "text/plain", "Target is already OFF.");
        }
    }
};
#endif
