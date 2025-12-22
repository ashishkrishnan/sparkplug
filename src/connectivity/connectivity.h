#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H
#include <Arduino.h>

class IConnection {
public:
    virtual ~IConnection() = default;
    virtual void setupConnectivity() = 0;
    virtual void handleConnectivityLoop() = 0;

    virtual bool isTargetPCAlive(const char* targetIp) = 0;
    virtual String getIpAddress() = 0;
    virtual String getMacAddress() = 0;
    virtual long getWifiSignalStrength() = 0;

    virtual float getInternalTemp() = 0;
    virtual String getChipModel() = 0;
    virtual uint32_t getFreeHeap() = 0;
    virtual uint32_t getTotalHeap() = 0;

    virtual String getFormattedTime() = 0;
    virtual String getUptime() = 0;
};

class Connectivity : public IConnection {
public:
    void setupConnectivity() override;
    void handleConnectivityLoop() override;

    bool isTargetPCAlive(const char* targetIp) override;
    String getIpAddress() override;
    String getMacAddress() override;
    long getWifiSignalStrength() override;

    float getInternalTemp() override;
    String getChipModel() override;
    uint32_t getFreeHeap() override;
    uint32_t getTotalHeap() override;

    String getFormattedTime() override;
    String getUptime() override;
};

#endif