#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H
#include <Arduino.h>

class IConnection {
public:
    virtual ~IConnection() = default;
    virtual void setupWifi() = 0;
    virtual void setupHostName() = 0;
    virtual void handleConnectivityLoop() = 0;

    virtual bool isTargetPCAlive(const char* targetIp) = 0;
    virtual String getIpAddress() = 0;
    virtual String getMacAddress() = 0;
    virtual long getWifiSignalStrength() = 0;
};

class Connectivity : public IConnection {
public:
    void setupWifi() override;

    void setupHostName() override;
    void handleConnectivityLoop() override;

    bool isTargetPCAlive(const char* targetIp) override;
    String getIpAddress() override;
    String getMacAddress() override;
    long getWifiSignalStrength() override;
};

#endif