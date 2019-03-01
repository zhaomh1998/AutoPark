//
// Created by Scott Zhao on 2019-02-21.
//
#ifndef ESP_AUTOPARKCONFIG_H
#define ESP_AUTOPARKCONFIG_H

#include <Arduino.h>
#include <vector>
#include <algorithm>

#define MASTER 0
#define FLOOR1 1
#define FLOOR2 2
#define FLOOR3 3
#define CAR1 4
#define CAR2 5
#define CAR3 6

class AutoParkConfig {
public:
//    uint8_t MAC_Master[6];
//    uint8_t MAC_Level1[6];
//    uint8_t MAC_Level2[6];
//    uint8_t MAC_Level3[6];
//    uint8_t MAC_Car1[6];
//    uint8_t MAC_Car2[6];
//    uint8_t MAC_Car3[6];
    uint8_t MAC_Master[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x33};
    uint8_t MAC_Level1[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x34};
    uint8_t MAC_Level2[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x35};
    uint8_t MAC_Level3[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x36};
    uint8_t MAC_Car1[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x37};
    uint8_t MAC_Car2[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x36};
    uint8_t MAC_Car3[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x39};
    std::vector<uint8_t *> macs;
    std::vector<String> deviceNames;

    AutoParkConfig() {
        macs = std::vector<uint8_t *>({MAC_Master, MAC_Level1, MAC_Level2, MAC_Level3, MAC_Car1, MAC_Car2, MAC_Car3});
        deviceNames = std::vector<String>({"Master", "Level1", "Level2", "Level3", "Car1", "Car2", "Car3"});
    }


    String whoIsThis(uint8_t *macAddr) {
//        Serial.print("Whoisthis Lookup!");
//        Serial.print("********");
//        logger::printMac(macAddr);
//        Serial.print("Size of macs:");
//        Serial.print(macs.size());
//        Serial.print(deviceNames.size());
//    logger::printMac(macs[4]);
        auto it = std::find(macs.begin(), macs.end(), macAddr);
        if (it == macs.end()) {
            return "UNKNOWN MAC";
        } else {
            auto index = std::distance(macs.begin(), it);
            return deviceNames[index];
        }
    }
};

#endif //ESP_AUTOPARKCONFIG_H
