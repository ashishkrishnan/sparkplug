#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H
#include <Arduino.h>

void setupConnectivity();

void handleConnectivityLoop();

bool isTargetPCAlive();

String getIpAddress();

long getWifiSignalStrength();

float getInternalTemp();

#endif //CONNECTIVITY_H
