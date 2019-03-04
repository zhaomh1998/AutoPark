//
// Created by Scott Zhao on 2019-02-18.
//

#ifndef ESP_MASTER_H
#define ESP_MASTER_H

#include "ESPNow.h"
#include <Arduino.h>

#define DEFAULT_MAX_CLIENT_AMOUNT 10


class master : public ESPNow {
public:

    master(uint8_t myMacIndex, bool debug = false) :
            ESPNow(myMacIndex, debug), maxClientAmount(DEFAULT_MAX_CLIENT_AMOUNT) {
        addAll(myMacIndex);
    }

    void addPeer(uint8_t deviceName) override {
        if (clientCount >= maxClientAmount) {
            log(ERROR, "Attempted to add ESPNow client more than defined maximum!\n");
        } else {
            clientCount++;
            ESPNow::setPeerMac(macs[deviceName], WIFI_CHANNEL);
            macList.emplace_back(macs[deviceName]);
        }
    }

    void addAll(uint8_t excludeIndex) {
        size_t deviceListLength = macs.size();
        for (uint8_t currentIndex = 0; currentIndex < deviceListLength; currentIndex++) {
            if (currentIndex == excludeIndex)
                continue;  // skip the exclude index
            addPeer(currentIndex);
        }
    }


private:
    std::vector<const uint8_t *> macList;
    size_t clientCount = {0};
    size_t maxClientAmount;
};


#endif //ESP_MASTER_H
