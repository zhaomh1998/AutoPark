//
// Created by Scott Zhao on 2019-02-18.
//

#ifndef ESP_SLAVE_H
#define ESP_SLAVE_H

#include <Arduino.h>
#include "ESPNow.h"

class slave : public ESPNow {
public:
    slave(uint8_t myMacIndex, uint8_t masterMacIndex, bool debug = false) : ESPNow(myMacIndex, debug),
                                                                            masterMacAddr(macs[masterMacIndex]),
                                                                            isDebugMode(debug) {
        ESPNow::addPeer(masterMacIndex);
    }

    void send(uint8_t *msg, uint8_t len) {
        ESPNow::send(masterMacAddr, msg, len);
        if (isDebugMode)
            printESPNowMsg(SEND, masterMacAddr, msg, len);
    }

protected:
    uint8_t *masterMacAddr;
    bool isDebugMode;
};


#endif //ESP_SLAVE_H
