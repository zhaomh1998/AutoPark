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
        auto it = std::find_if(macs.begin(), macs.end(),
                               [&macAddr](const uint8_t *nthMac) { return *nthMac == *macAddr; });
        if (it == macs.end()) {
            return "UNKNOWN MAC";
        } else {
            auto index = std::distance(macs.begin(), it);
            return deviceNames[index];
        }
    }

    void printMac(uint8 *macAddr) {
        for (uint8_t macIndex = 0; macIndex < 6; macIndex++) {
            Serial.printf("%x ", macAddr[macIndex]);
        }
    }
};

#endif //ESP_AUTOPARKCONFIG_H
