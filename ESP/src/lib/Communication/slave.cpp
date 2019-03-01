//
// Created by Scott Zhao on 2019-02-18.
//

#include "slave.h"

slave::slave(uint8_t *myMacAddr, uint8_t *masterMacAddr, bool debug) : ESPNow(myMacAddr, debug),
                                                                       masterMacAddr(masterMacAddr),
                                                                       isDebugMode(debug) {
    ESPNow::addPeer(masterMacAddr);
}