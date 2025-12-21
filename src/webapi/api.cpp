#ifndef API_CPP
#define API_CPP
#include "../../src/webapi/webapi.h"
#include "../../src/config/config.h"
#include "../../src/connectivity/connectivity.cpp"
#include "WebServer.h"
#include "WiFiUdp.h"

class Api : public WebApi {
    WebServer server{
      HTTP_PORT
    };

    WiFiUDP Udp;
    uint8_t packetBuffer[102];
    WakeCallback wakeCb;
    ShutDownCallback shutdownCb;

    String logs[MAX_LOGS];
    int logIdx = 0;
    bool wrapped = false;

  Connectivity getConn() {
        return Connectivity();
    }

public:
  void logEvent(String msg) override {
      String entry = "[" + String(millis()/1000) + "s] " + msg;
      logs[logIdx] = entry;
      logIdx++;
      if (logIdx >= MAX_LOGS) { logIdx=0; wrapped=true; }
      Serial.println(entry);
    }

  bool isThermalUnsafe() {
      float currentTemp = getConn().getInternalTemp();
      if (currentTemp > MAX_TEMP_C) {
        logEvent("CRITICAL: Temp " + String(currentTemp) + "C exceeds limit!");
        return true;
      }
      return false;
    }

  void handleHealth() {
      String json = "{";
      json += "\"status\":\"online\",";
      json += "\"ip\":\"" + getConn().getIpAddress() + "\",";
      json += "\"signal_dbm\":" + String(getConn().getWifiSignalStrength()) + ",";
      json += "\"temp_c\":" + String(getConn().getInternalTemp()) + ",";
      json += "\"logs\":[";
      int count = wrapped ? MAX_LOGS : logIdx;
      int start = wrapped ? logIdx : 0;
      for(int i=0; i<count; i++) {
        int idx = (start + i) % MAX_LOGS;
        json += "\"" + logs[idx] + "\"";
        if(i < count-1) json += ",";
      }
      json += "]}";
      server.send(200, "application/json", json);
    }

    void handleWake() {
      String os = server.hasArg("os") ? server.arg("os") : OS_NAME_PRIMARY;

      if(isThermalUnsafe()) {
        server.send(503, "text/plain", "Thermal Lockout. Device too hot.");
        return;
      }

      logEvent("API Wake Req: " + os);
      server.send(200, "text/plain", "Sparking " + os);
      if(wakeCb) wakeCb(os);
    }

    void handleShutdown() {
      logEvent("API Shutdown Req. Checking Ping...");

      if(isThermalUnsafe()) {
        server.send(503, "text/plain", "Thermal Lockout. Device too hot.");
        return;
      }

      if(getConn().isTargetPCAlive(TARGET_PC_IP_ADDRESS)) {
        logEvent("Target ON. Shutting down.");
        server.send(200, "text/plain", "Executing Safe Shutdown");
        if(shutdownCb) shutdownCb();
      } else {
        logEvent("Target unreachable (OFF). Aborting.");
        server.send(409, "text/plain", "Target OFF. Aborted.");
      }
    }

    void checkWoL() {
      int size = Udp.parsePacket();
      if (size == 102) {
        Udp.read(packetBuffer, 102);
        if(packetBuffer[0] == 0xFF) {
          logEvent("WoL Recv from " + Udp.remoteIP().toString());
          if(!isThermalUnsafe() && wakeCb) wakeCb(OS_NAME_PRIMARY);
        }
      }
    }

    void setupWebAPI(WakeCallback onWake, ShutDownCallback onShutdown) { // Fixed typo
      wakeCb = onWake;
      shutdownCb = onShutdown;

      server.on("/wake", HTTP_POST, [this](){ handleWake(); });
      server.on("/shutdown", HTTP_POST, [this](){ handleShutdown(); });
      server.on("/health", HTTP_GET, [this](){ handleHealth(); });

      server.begin();
      Udp.begin(WOL_PORT);
      logEvent("WebAPI Started");
    }

    void handleWebAPILoop() {
      server.handleClient();
      checkWoL();
    }

};

#endif
