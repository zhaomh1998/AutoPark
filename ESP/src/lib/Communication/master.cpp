//
// Created by Scott Zhao on 2019-02-18.
//

#include "master.h"

void master::add_peer(uint8_t *mac, u8 channel) {
    if (clientCount >= maxClientAmount) {
        logHandle.error("Attempted to add ESPNow client more than defined maximum!");
    } else {
        clientCount++;
        esp_now_add_peer(mac, ESP_NOW_ROLE_SLAVE, channel, nullptr, 0);
        macList.emplace_back(mac);
    }
}

int master::searchMac(const uint8_t *inMac) {
    auto it = std::find(macList.begin(), macList.end(), inMac);
    if (it == macList.end()) {
        return -1;
    } else {
        auto index = std::distance(macList.begin(), it);
    }
}

void master::addAll(const std::vector<uint8_t *> macAddressList, uint8_t excludeIndex) {
    uint8_t currentIndex = 0;
    for (auto it = macAddressList.begin(); it != macAddressList.end(); it++, currentIndex++) {
        if (currentIndex == excludeIndex)
            continue;  // skip the exclude index
        add_peer(*it);
    }
}
