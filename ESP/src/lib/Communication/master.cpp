//
// Created by Scott Zhao on 2019-02-18.
//

#include "master.h"

void master::addPeer(uint8_t deviceName) {
    if (clientCount >= maxClientAmount) {
        logHandle.error("Attempted to add ESPNow client more than defined maximum!");
    } else {
        clientCount++;
        ESPNow::setPeerMac(macs[deviceName], WIFI_CHANNEL);
        macList.emplace_back(macs[deviceName]);
    }
}

void master::addAll(uint8_t excludeIndex) {
    size_t listLength = macs.size();
    for (uint8_t currentIndex = 0; currentIndex < listLength; currentIndex++) {
        if (currentIndex == excludeIndex)
            continue;  // skip the exclude index
        addPeer(currentIndex);
    }
}
