#ifndef WOLVIRTUALADDRESS_H
#define WOLVIRTUALADDRESS_H

class WolVirtualAddress {
public:
    static String getVirtualMacString(const uint8_t *baseMac, uint8_t suffix) {
        char macStr[18];
        snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
                 baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], suffix);
        return String(macStr);
    }
};

#endif
