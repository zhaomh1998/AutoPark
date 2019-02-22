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

    master(uint8_t *myMacAddr, size_t maxClientAmount = DEFAULT_MAX_CLIENT_AMOUNT, bool debug = false) :
            ESPNow(myMacAddr, debug), maxClientAmount(maxClientAmount) {}

    int searchMac(const uint8_t *inMac);

    void add_peer(uint8_t *mac, u8 channel = WIFI_CHANNEL) override;

    void addAll(const std::vector<uint8_t *> macAddressList, uint8_t excludeIndex);

private:
    std::vector<const uint8_t *> macList;
    size_t clientCount = {0};
    size_t maxClientAmount;
};


#endif //ESP_MASTER_H
