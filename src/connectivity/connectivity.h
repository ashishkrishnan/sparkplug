#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H
#include <Arduino.h>
#include <WiFi.h>

class IConnection {
public:
    virtual ~IConnection() = default;
    virtual void setupWifi() = 0;
    virtual void setupHostName() = 0;
    virtual void handleConnectivityLoop() = 0;

    virtual bool isTargetPCAlive() = 0;
    virtual String getIpAddress() = 0;
    virtual String getMacAddress() = 0;
    virtual void getMacBytes(uint8_t* buf);
    virtual long getWifiSignalStrength() = 0;
};

class Connectivity : public IConnection {
public:
    void setupWifi() override;

    void setupHostName() override;
    void handleConnectivityLoop() override;

    bool isTargetPCAlive() override;
    String getIpAddress() override;
    String getMacAddress() override;
    void getMacBytes(uint8_t* buf) override;

    long getWifiSignalStrength() override;
};

#endif