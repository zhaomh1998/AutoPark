//
// Created by Scott Zhao on 2019-02-18.
//

#include "slave.h"

slave::slave(uint8_t *myMacAddr, uint8_t *masterMacAddr, bool debug) : ESPNow(myMacAddr, debug),
                                                                       masterMacAddr(masterMacAddr) {
    ESPNow::addPeer(masterMacAddr);
}

void slave::messageHandler(uint8_t *mac, uint8_t *data, uint8_t len) {
    // TODO: command handler
}
