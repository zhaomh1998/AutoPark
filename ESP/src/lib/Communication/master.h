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

    master(uint8_t deviceName, bool debug = false) :
            ESPNow(deviceName, debug), maxClientAmount(DEFAULT_MAX_CLIENT_AMOUNT) {
        addAll(deviceName);
    }

    uint8_t searchMac(const uint8_t *inMac);

    void addPeer(uint8_t deviceName) override;

    void addAll(uint8_t excludeIndex);

private:
    std::vector<const uint8_t *> macList;
    size_t clientCount = {0};
    size_t maxClientAmount;
};


#endif //ESP_MASTER_H
