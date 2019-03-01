//
// Created by Scott Zhao on 2019-02-23.
//
#include "AutoParkConfig.h"
#include "logger.h"
#include "AutoParkConfig.h"

uint8_t MAC_Master[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x33};
uint8_t MAC_Level1[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x34};
uint8_t MAC_Level2[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x35};
uint8_t MAC_Level3[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x36};
uint8_t MAC_Car1[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x37};
uint8_t MAC_Car2[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x36};
uint8_t MAC_Car3[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x39};


String whoIsThis(uint8_t *macAddr) {
    Serial.println("Whoisthis Lookup!");
    Serial.println("********");
    logger::printMac(macAddr);
    Serial.println("Size of macs:");
    Serial.print(macs.size());
    Serial.print(deviceNames.size());
//    logger::printMac(macs[4]);
    Serial.println();
    auto it = std::find(macs.begin(), macs.end(), macAddr);
    if (it == macs.end()) {
        return "UNKNOWN MAC";
    } else {
        auto index = std::distance(macs.begin(), it);
        return deviceNames[index];
    }
}